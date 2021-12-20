#include "spatial/resources/ResourceLoader.h"
#include <boost/algorithm/string/predicate.hpp>
#include <fstream>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>

namespace spatial::render
{

MeshController::MeshController(filament::Engine& engine)
	: mEngine{engine}, mRoot{}, mVertexBuffers{}, mIndexBuffers{}, mMeshGeometries{}, mBoundingBoxes{}
{
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

void MeshController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	populateMeshesDatabase(registry);
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	clearDeletedOrDirtyMeshes(registry);
}

void MeshController::createRenderableMeshes(ecs::Registry& registry)
{
	registry.getEntities<const Entity, ecs::Mesh>(ecs::ExcludeComponents<Renderable>)
		.each([&](ecs::Entity e, const auto& entity, const auto& mesh) {
			const auto resourceId = mesh.getResourceId();

			if (!hasMeshData(mesh.getResourceId()))
				return;

			const auto& parts = mMeshGeometries.at(resourceId);
			const auto partsCount = mesh.partsCount == 0 ? parts.size() : mesh.partsCount;
			registry.addComponent<Renderable>(e, mEngine, entity.get(), partsCount);
		});
}

void MeshController::updateMeshGeometries(ecs::Registry& registry)
{
	registry.getEntities<const ecs::Mesh, Renderable>().each([&, this](const auto& mesh, auto& renderable) {
		const auto resourceId = mesh.getResourceId();

		if (!hasMeshData(resourceId))
			return;

		const auto& parts = mMeshGeometries.at(resourceId);
		auto& vertexBuffer = mVertexBuffers.at(resourceId);
		auto& indexBuffer = mIndexBuffers.at(resourceId);
		auto& boundingBox = mBoundingBoxes.at(resourceId);

		renderable.setAxisAlignedBoundingBox(boundingBox);
		renderable.setCastShadows(mesh.castShadows);
		renderable.setReceiveShadows(mesh.receiveShadows);
		renderable.setCulling(mesh.culling);

		const auto partsCount = mesh.partsCount == 0 ? parts.size() : mesh.partsCount;

		for (auto i = 0; i < std::min(partsCount, parts.size()); i++)
		{
			const auto& geometry = parts[std::min(mesh.partsOffset + i, parts.size() - 1)];
			renderable.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer.get(), indexBuffer.get(),
									 geometry.offset, geometry.count);

			if (registry.hasAnyComponent<MaterialInstance>(mesh.defaultMaterial))
			{
				const auto& materialInstance = registry.getComponent<const MaterialInstance>(mesh.defaultMaterial);
				renderable.setMaterialInstanceAt(i, materialInstance.get());
			}
			else if (mDefaultMaterialInstance.get())
			{
				renderable.setMaterialInstanceAt(i, mDefaultMaterialInstance.get());
			}
		}
	});
}

bool MeshController::hasMeshData(MeshId resourceId) const
{
	return mMeshGeometries.find(resourceId) != mMeshGeometries.end();
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

tl::expected<FilameshFile, ResourceError> toFilamesh(std::istream&& istream)
{
	try
	{
		auto filamesh = FilameshFile{};
		istream >> filamesh;
		return filamesh;
	}
	catch (const std::ios::failure& e)
	{
		return tl::make_unexpected(ResourceError::ParseError);
	}
}

void MeshController::populateMeshesDatabase(ecs::Registry& registry)
{
	using namespace boost::algorithm;

	registry.getEntities<const ecs::Mesh>().each([this](auto& mesh) {
		const auto resourceId = mesh.getResourceId();

		if (hasMeshData(resourceId))
			return;

		if (!ends_with(mesh.resourcePath.string(), ".filamesh"))
			return;

		auto result = makeAbsolutePath(mRoot, mesh.resourcePath)
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