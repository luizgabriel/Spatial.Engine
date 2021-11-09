#include <c++/v1/fstream>
#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>

namespace spatial::render
{

MeshController::MeshController(filament::Engine& engine, std::filesystem::path root)
	: mEngine{engine}, mRoot{root}, mVertexBuffers{}, mIndexBuffers{}, mMeshGeometries{}, mBoundingBoxes{}
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
	loadMeshes(registry);
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	clearDeletedMeshes(registry);
}

void MeshController::createRenderableMeshes(ecs::Registry& registry)
{
	auto view = registry.getEntities<ecs::Mesh>(ecs::ExcludeComponents<Renderable>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);

		registry.addComponent<Renderable>(entity, mEngine, renderable.get(), data.partsCount);
	}
}

void MeshController::updateMeshGeometries(ecs::Registry& registry)
{
	auto view = registry.getEntities<ecs::Mesh, Renderable>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);
		const auto resourceId = toMeshId(data.resourcePath);

		if (!hasMeshData(resourceId))
			continue;

		const auto& parts = mMeshGeometries.at(resourceId);
		auto& vertexBuffer = mVertexBuffers.at(resourceId);
		auto& indexBuffer = mIndexBuffers.at(resourceId);
		auto& boundingBox = mBoundingBoxes.at(resourceId);

		auto& renderableMesh = registry.getComponent<Renderable>(entity);

		renderableMesh.setAxisAlignedBoundingBox(boundingBox);
		renderableMesh.setCastShadows(data.castShadows);
		renderableMesh.setReceiveShadows(data.receiveShadows);
		renderableMesh.setCulling(data.culling);

		for (auto i = 0; i < data.partsCount; i++)
		{
			const auto& geometry = parts[data.partsOffset + i];
			renderableMesh.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer.get(), indexBuffer.get(),
										 geometry.offset, geometry.count);

			ecs::Entity materialEntity;
			if (i > ecs::Mesh::MAX_GEOMETRIES - 1 || !registry.isValid(data.materials[i]))
				materialEntity = data.defaultMaterial;
			else
				materialEntity = data.materials[i];

			if (registry.hasAllComponents<MaterialInstance>(materialEntity))
			{
				const auto& materialInstance = registry.getComponent<const MaterialInstance>(materialEntity);
				renderableMesh.setMaterialInstanceAt(i, materialInstance.get());
			}
		}
	}
}

bool MeshController::hasMeshData(MeshId resourceId) const
{
	return mMeshGeometries.find(resourceId) != mMeshGeometries.end();
}

void MeshController::clearDeletedMeshes(ecs::Registry& registry)
{
	auto view = registry.getEntities<Renderable>(ecs::ExcludeComponents<ecs::Mesh>);
	registry.removeComponent<Renderable>(view.begin(), view.end());
}

uint32_t MeshController::toMeshId(const std::filesystem::path& path)
{
	return HashedString{path.string().c_str()}.value();
}

void MeshController::loadMeshes(ecs::Registry& registry)
{
	auto view = registry.getEntities<const ecs::Mesh>();
	view.each([this](auto& mesh) {
		const auto path = mesh.resourcePath;
		const auto resourceId = toMeshId(mesh.resourcePath);
		if (!hasMeshData(resourceId))
		{
			std::thread{[=]() {
				auto fs = std::ifstream{mRoot / path};
				if (!fs) return;

				auto mesh = FilameshFile{};
				fs >> mesh;
				load(resourceId, mesh);
			}};
		}
	});
}

} // namespace spatial::render