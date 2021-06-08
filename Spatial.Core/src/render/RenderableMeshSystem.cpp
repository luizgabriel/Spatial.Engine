#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/RenderableMesh.h>
#include <spatial/render/RenderableMeshSystem.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

RenderableMeshSystem::RenderableMeshSystem(filament::Engine& engine) : mEngine{engine}
{
}

void RenderableMeshSystem::synchronize(ecs::Registry& registry) const
{
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	updateMeshMaterials(registry);
}

void RenderableMeshSystem::updateMeshMaterials(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::MeshRenderer, RenderableMesh>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::MeshRenderer>(entity);
		auto& renderableMesh = registry.getComponent<RenderableMesh>(entity);

		renderableMesh.setCastShadows(data.castShadows);
		renderableMesh.setReceiveShadows(data.receiveShadows);

		for (auto i = 0; i < data.materialsCount; i++)
		{
			const auto& materialInstance = registry.getComponent<MaterialInstance>(data.materials[i]);
			renderableMesh.setMaterialInstanceAt(i, materialInstance.get());
		}
	}
}

void RenderableMeshSystem::updateMeshGeometries(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Mesh, RenderableMesh>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);
		auto& vertexBuffer = registry.getComponent<VertexBuffer>(data.vertexBufferId);
		auto& indexBuffer = registry.getComponent<IndexBuffer>(data.indexBufferId);
		auto& renderableMesh = registry.getComponent<RenderableMesh>(entity);

		for (auto i = 0; i < data.geometriesCount; i++)
		{
			const auto& geometry = data.geometries[i];
			renderableMesh.setGeometryAt(i, RenderableMesh::PrimitiveType::TRIANGLES, vertexBuffer.get(),
										 indexBuffer.get(), geometry.offset, geometry.count);
		}
	}
}

void RenderableMeshSystem::createRenderableMeshes(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Mesh>(ecs::ExcludeComponents<RenderableMesh>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		const auto& data = registry.getComponent<const ecs::Mesh>(entity);

		registry.addComponent<RenderableMesh>(entity, mEngine, renderable.get(), data.geometriesCount);
	}
}


} // namespace spatial::render