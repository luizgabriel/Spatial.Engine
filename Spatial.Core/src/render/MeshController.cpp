#include "spatial/ecs/Relation.h"
#include <array>
#include <boost/algorithm/string/predicate.hpp>
#include <filament/Engine.h>
#include <fstream>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/resources/FilameshFile.h>
#include <spatial/resources/ResourceLoader.h>

namespace spatial::render
{

static constexpr std::array<math::float4, 3> sFullScreenTriangleVertices = {math::float4{-1.0f, -1.0f, 1.0f, 1.0f},
																			math::float4{3.0f, -1.0f, 1.0f, 1.0f},
																			math::float4{-1.0f, 3.0f, 1.0f, 1.0f}};

static constexpr std::array<uint16_t, 3> sFullScreenTriangleIndices = {0, 1, 2};

MeshController::MeshController(filament::Engine& engine)
	: mEngine{engine}, mRoot{}, mVertexBuffers{}, mIndexBuffers{}, mMeshGeometries{}, mBoundingBoxes{}
{
	auto* vb = filament::VertexBuffer::Builder()
				   .vertexCount(3)
				   .bufferCount(1)
				   .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT4, 0)
				   .build(mEngine);

	vb->setBufferAt(mEngine, 0, {sFullScreenTriangleVertices.data(), sizeof(sFullScreenTriangleVertices)});

	mVertexBuffers.emplace("engine://fullscreen"_hs, render::VertexBuffer{mEngine, vb});

	auto* ib = filament::IndexBuffer::Builder()
				   .indexCount(3)
				   .bufferType(filament::IndexBuffer::IndexType::USHORT)
				   .build(mEngine);
	ib->setBuffer(mEngine, {sFullScreenTriangleIndices.data(), sizeof(sFullScreenTriangleIndices)});

	mIndexBuffers.emplace("engine://fullscreen"_hs, render::IndexBuffer{mEngine, ib});

	mMeshGeometries.emplace("engine://fullscreen"_hs, std::vector<MeshGeometry>{MeshGeometry{0, vb->getVertexCount()}});
}

void MeshController::load(MeshId resourceId, const FilameshFile& filamesh)
{
	mVertexBuffers.emplace(resourceId, createVertexBuffer(mEngine, filamesh));
	mIndexBuffers.emplace(resourceId, createIndexBuffer(mEngine, filamesh));
	mBoundingBoxes.emplace(resourceId, filamesh.header.aabb);

	auto geometries = std::vector<MeshGeometry>(filamesh.parts.size());

	for (auto i = 0; i < filamesh.parts.size(); i++)
	{
		const auto& part = filamesh.parts[i];
		geometries.at(i) = MeshGeometry{part.offset, part.count};
	}

	mMeshGeometries.emplace(resourceId, std::move(geometries));
}

void MeshController::onUpdateFrame(ecs::Registry& registry)
{
	populateMeshesDatabase(registry);
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	clearDeletedOrDirtyMeshes(registry);
}

void MeshController::createRenderableMeshes(ecs::Registry& registry)
{
	registry.getEntities<const Entity, const ecs::Mesh>(ecs::ExcludeComponents<Renderable>)
		.each([&](ecs::Entity e, const auto& entity, const auto& mesh) {
			const auto resourceId = mesh.meshResource.getId();

			if (!hasMeshData(resourceId))
				return;

			const auto& parts = mMeshGeometries.at(resourceId);
			const auto partsCount = mesh.partsCount == 0 ? parts.size() : mesh.partsCount;
			registry.addComponent<Renderable>(e, mEngine, entity.get(), partsCount);
		});
}

void MeshController::updateMeshGeometries(ecs::Registry& registry)
{
	registry.getEntities<const ecs::Mesh, Renderable>().each(
		[&, this](ecs::Entity entity, const auto& mesh, auto& renderable) {
			const auto resourceId = mesh.meshResource.getId();

			if (!hasMeshData(resourceId))
				return;

			const auto& parts = mMeshGeometries.at(resourceId);
			auto& vertexBuffer = mVertexBuffers.at(resourceId);
			auto& indexBuffer = mIndexBuffers.at(resourceId);

			auto boundingBoxIt = mBoundingBoxes.find(resourceId);
			if (boundingBoxIt != mBoundingBoxes.end())
				renderable.setAxisAlignedBoundingBox(boundingBoxIt->second);

			renderable.setCastShadows(mesh.castShadows);
			renderable.setReceiveShadows(mesh.receiveShadows);
			renderable.setCulling(mesh.culling);
			renderable.setPriority(mesh.priority);

			const auto partsCount = mesh.partsCount == 0 ? parts.size() : mesh.partsCount;

			for (auto i = 0; i < std::min(partsCount, parts.size()); i++)
			{
				const auto& geometry = parts[std::min(mesh.partsOffset + i, parts.size() - 1)];
				renderable.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer.get(), indexBuffer.get(),
										 geometry.offset, geometry.count);
				renderable.setMaterialInstanceAt(i, mDefaultMaterialInstance.get());
			}

			if (registry.template hasAllComponents<ecs::Parent>(entity))
			{
				ecs::Parent::forEachChild(registry, entity, [&](ecs::Entity child) {
					const auto* meshMaterial = registry.tryGetComponent<const ecs::MeshMaterial>(child);
					if (meshMaterial != nullptr)
					{
						const auto& materialInstance =
							registry.getComponent<const MaterialInstance>(meshMaterial->materialEntity);
						renderable.setMaterialInstanceAt(meshMaterial->primitiveIndex, materialInstance.get());
					}
				});
			}
		});
}

bool MeshController::hasMeshData(MeshId resourceId) const
{
	return mVertexBuffers.find(resourceId) != mVertexBuffers.end()
		   && mIndexBuffers.find(resourceId) != mIndexBuffers.end()
		   && mMeshGeometries.find(resourceId) != mMeshGeometries.end();
}

void MeshController::clearDeletedOrDirtyMeshes(ecs::Registry& registry)
{
	auto view = registry.getEntities<Renderable>(ecs::ExcludeComponents<ecs::Mesh>);
	registry.removeComponent<Renderable>(view.begin(), view.end());
}

void MeshController::setRootPath(const std::filesystem::path& root)
{
	mRoot = root;
}

void MeshController::populateMeshesDatabase(ecs::Registry& registry)
{
	using namespace boost::algorithm;

	registry.getEntities<const ecs::Mesh>().each([this](const auto& mesh) {
		const auto resourceId = mesh.meshResource.getId();

		if (hasMeshData(resourceId))
			return;

		if (!ends_with(mesh.meshResource.relativePath.c_str(), ".filamesh"))
			return;

		auto result = makeAbsolutePath(mRoot, mesh.meshResource.relativePath)
						  .and_then(validateResourcePath)
						  .and_then(openFileReadStream)
						  .and_then(toFilamesh)
						  .map_error(logResourceError);

		if (!result.has_value())
			return;

		load(resourceId, result.value());
	});
}

void MeshController::setDefaultMaterialInstance(const SharedMaterialInstance& defaultMaterialInstance)
{
	mDefaultMaterialInstance = defaultMaterialInstance;
}

} // namespace spatial::render