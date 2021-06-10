#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/RenderableMeshSystem.h>
#include <spatial/render/Resources.h>
#include <spatial/render/ResourceLoaders.h>

namespace spatial::render
{

RenderableMeshSystem::RenderableMeshSystem(filament::Engine& engine) : mEngine{engine}, mVertexBuffers{}, mIndexBuffers{}, mMeshGeometries{}, mBoundingBoxes{}
{
}

void RenderableMeshSystem::define(const HashedString& resourceName, const FilameshFile& filamesh)
{
	auto resourceId = resourceName.value();
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

void RenderableMeshSystem::synchronize(ecs::Registry& registry)
{
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	updateMeshMaterials(registry);
	clearDeletedMeshes(registry);
}

void RenderableMeshSystem::updateMeshMaterials(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Mesh, Renderable>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);
		const auto resourceId = HashedString{data.resourceName.data()}.value();

		if (!hasMeshData(resourceId))
			continue;

		auto& renderableMesh = registry.getComponent<Renderable>(entity);

		renderableMesh.setCastShadows(data.castShadows);
		renderableMesh.setReceiveShadows(data.receiveShadows);

		const auto& parts = mMeshGeometries.at(resourceId);
		for (auto i = 0; i < data.partsCount; i++)
		{
			auto materialEntity = data.materials[i];
			if (registry.isValid(materialEntity) && registry.hasAllComponents<MaterialInstance>(materialEntity)) {
				const auto& materialInstance = registry.getComponent<const MaterialInstance>(materialEntity);
				renderableMesh.setMaterialInstanceAt(i, materialInstance.get());
			}
		}
	}
}


void RenderableMeshSystem::createRenderableMeshes(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Mesh>(ecs::ExcludeComponents<Renderable>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);

		registry.addComponent<Renderable>(entity, mEngine, renderable.get(), data.partsCount);
	}
}

void RenderableMeshSystem::updateMeshGeometries(ecs::Registry& registry)
{
	auto view = registry.getEntities<ecs::Mesh, Renderable>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);
		const auto resourceId = HashedString{data.resourceName.data()}.value();

		if (!hasMeshData(resourceId))
			continue;

		const auto& parts = mMeshGeometries.at(resourceId);
		auto& vertexBuffer = mVertexBuffers.at(resourceId);
		auto& indexBuffer = mIndexBuffers.at(resourceId);
		auto& boundingBox = mBoundingBoxes.at(resourceId);

		auto& renderableMesh = registry.getComponent<Renderable>(entity);

		renderableMesh.setAxisAlignedBoundingBox(boundingBox);

		for (auto i = 0; i < data.partsCount; i++)
		{
			const auto& geometry = parts[data.partsOffset + i];
			renderableMesh.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer.get(),
										 indexBuffer.get(), geometry.offset, geometry.count);
		}
	}
}

bool RenderableMeshSystem::hasMeshData(HashedString::hash_type resourceId) const
{
	return mMeshGeometries.find(resourceId) != mMeshGeometries.end();
}

void RenderableMeshSystem::clearDeletedMeshes(ecs::Registry& registry)
{
	auto view = registry.getEntities<Renderable>(ecs::ExcludeComponents<ecs::Mesh>);
	registry.removeComponent<Renderable>(view.begin(), view.end());
}

} // namespace spatial::render