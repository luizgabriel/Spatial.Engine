#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

MeshController::MeshController(filament::Engine& engine) : mEngine{engine}, mVertexBuffers{}, mIndexBuffers{}, mMeshGeometries{}, mBoundingBoxes{}
{
}

void MeshController::load(std::uint32_t resourceId, const FilameshFile& filamesh)
{
	mVertexBuffers.emplace(resourceId, createVertexBuffer(mEngine, filamesh.header, filamesh.vertexData));
	mIndexBuffers.emplace(resourceId, createIndexBuffer(mEngine, filamesh.header, filamesh.indexData));
	mBoundingBoxes.emplace(resourceId, filamesh.header.aabb);

	auto geometries = std::vector<MeshGeometry>(filamesh.parts.size());

	for (auto i = 0; i < filamesh.parts.size(); i++) {
		const auto& part = filamesh.parts[i];
		geometries.at(i) = MeshGeometry{
			part.offset,
			part.count
		};
	}

	mMeshGeometries.emplace(resourceId, std::move(geometries));
}

void MeshController::unload(std::uint32_t resourceId)
{
	mVertexBuffers.erase(resourceId);
	mIndexBuffers.erase(resourceId);
	mBoundingBoxes.erase(resourceId);
	mMeshGeometries.erase(resourceId);
}

void MeshController::onUpdateFrame(ecs::Registry& registry)
{
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	updateMeshMaterials(registry);
	clearDeletedMeshes(registry);
}

void MeshController::updateMeshMaterials(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Mesh, Renderable>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);

		if (!hasMeshData(data.resourceId))
			continue;

		auto& renderableMesh = registry.getComponent<Renderable>(entity);




	}
}


void MeshController::createRenderableMeshes(ecs::Registry& registry) const
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

		if (!hasMeshData(data.resourceId))
			continue;

		const auto& parts = mMeshGeometries.at(data.resourceId);
		auto& vertexBuffer = mVertexBuffers.at(data.resourceId);
		auto& indexBuffer = mIndexBuffers.at(data.resourceId);
		auto& boundingBox = mBoundingBoxes.at(data.resourceId);

		auto& renderableMesh = registry.getComponent<Renderable>(entity);

		renderableMesh.setAxisAlignedBoundingBox(boundingBox);
		renderableMesh.setCastShadows(data.castShadows);
		renderableMesh.setReceiveShadows(data.receiveShadows);
		renderableMesh.setCulling(data.culling);

		for (auto i = 0; i < data.partsCount; i++)
		{
			const auto& geometry = parts[data.partsOffset + i];
			renderableMesh.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer.get(),
										 indexBuffer.get(), geometry.offset, geometry.count);

			ecs::Entity materialEntity;
			if (i > 31 || !registry.isValid(data.materials[i]))
				materialEntity = data.defaultMaterial;
			else
				materialEntity = data.materials[i];

			if (registry.hasAllComponents<MaterialInstance>(materialEntity)) {
				const auto& materialInstance = registry.getComponent<const MaterialInstance>(materialEntity);
				renderableMesh.setMaterialInstanceAt(i, materialInstance.get());
			}
		}

	}
}

bool MeshController::hasMeshData(HashedString::hash_type resourceId) const
{
	return mMeshGeometries.find(resourceId) != mMeshGeometries.end();
}

void MeshController::clearDeletedMeshes(ecs::Registry& registry)
{
	auto view = registry.getEntities<Renderable>(ecs::ExcludeComponents<ecs::Mesh>);
	registry.removeComponent<Renderable>(view.begin(), view.end());
}

} // namespace spatial::render