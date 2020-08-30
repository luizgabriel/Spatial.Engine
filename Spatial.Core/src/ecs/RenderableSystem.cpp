#include <spatial/ecs/RenderableSystem.h>

namespace spatial::ecs
{

RenderableSystem::RenderableSystem(MaterialRegistry& materialRegistry, MeshRegistry& meshRegistry)
	: mMaterialRegistry{materialRegistry}, mMeshRegistry{meshRegistry}, mRenderableManager{materialRegistry.getEngine().getRenderableManager()}
{
}

void RenderableSystem::onUpdate(entt::registry& registry)
{
	const auto materialsView = registry.view<const ecs::SceneEntity, const ecs::Renderable>();
	materialsView.each([&](const ecs::SceneEntity& sceneEntity, const ecs::Renderable& renderable) {
	  auto instance = mRenderableManager.getInstance(sceneEntity.entity);

	  mRenderableManager.setCastShadows(instance, renderable.castShadows);
	  mRenderableManager.setReceiveShadows(instance, renderable.receiveShadows);

	  auto& mesh = mMeshRegistry.at(renderable.meshId);
	  auto& material = mMaterialRegistry.at(renderable.materialId);
	  mRenderableManager.setMaterialInstanceAt(instance, renderable.subMeshId, material.get())
	});
}

void RenderableSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	auto& sceneEntity = registry.get_or_emplace<ecs::SceneEntity>(entity);
	auto& renderable = registry.get<ecs::Renderable>(entity);

	auto& mesh = mMeshRegistry.at(renderable.meshId);
	auto& material = mMaterialRegistry.at(renderable.materialId);

	const auto& part = mesh[renderable.subMeshId];

	auto builder = fl::RenderableManager::Builder(mesh.size())
					   .boundingBox(mesh.boundingBox())
					   .geometry(renderable.subMeshId, fl::RenderableManager::PrimitiveType::TRIANGLES, mesh.getVertexBuffer().get(),
						 mesh.getIndexBuffer().get(), mesh[renderable.subMeshId].offset, mesh[renderable.subMeshId].minIndex, mesh[renderable.subMeshId].maxIndex, mesh[renderable.subMeshId].indexCount);

		if (map.find(part.materialName) != map.end())
			builder.material(index, map.at(part.materialName));
		else
			builder.material(index, map.at("DefaultMaterial"));

		index++;
	}

	builder.build(getEngine(), entity);
}

void RenderableSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
}

}