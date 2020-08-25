#include <spatial/ecs/Components.h>
#include <spatial/ecs/RenderableSystem.h>
#include <utils/EntityManager.h>

namespace spatial::ecs
{

RenderableSystem::RenderableSystem(filament::Engine& engine, filament::Scene& scene)
	: mEngine{engine}, mScene{scene}, mMaterialInstances{}
{
}

void RenderableSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	auto& component = registry.get<Renderable>(entity);
	mScene.addEntity(component.entity.get());
}

void RenderableSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	auto& component = registry.get<Renderable>(entity);
	mScene.remove(component.entity.get());
	mMaterialInstances.erase(entity);
}

void RenderableSystem::buildMaterialInstances(const entt::registry& registry, const MaterialSystem& materialSystem)
{
	const auto materialsView = registry.view<const ecs::Material>();
	materialsView.each([this, &registry, &materialSystem](const ecs::Material& component) {
		auto& material = materialSystem.get(component.resourceFilePath);
		if (registry.valid(component.parentShape))
		{
			if (!mMaterialInstances.contains(component.parentShape)) {
				auto materialDict = std::unordered_map<std::string, MaterialInstance>(1);
				mMaterialInstances.emplace(component.parentShape, std::move(materialDict));
			}

			auto materialInstance = createMaterialInstance(mEngine, material.ref(), component.name.c_str());
			mMaterialInstances.at(component.parentShape).emplace(component.name, std::move(materialInstance));
		}
	});
}

void RenderableSystem::buildShapeRenderables(entt::registry& registry, MeshSystem& meshSystem)
{
	auto shapesView = registry.view<ecs::Mesh>();
	for (entt::entity entity : shapesView)
	{
		const auto& component = shapesView.get<ecs::Mesh>(entity);
		auto renderable = createEntity(mEngine);

		auto& mesh = meshSystem.get(component.resourceFilePath);
		auto& materialInstances = mMaterialInstances.at(entity);
		auto materialDict = spatial::Mesh::MaterialsMap(materialInstances.size());
		for (auto& [materialName, materialInstance] : materialInstances) {
			materialDict.emplace(materialName, materialInstance.get());
		}

		mesh.build(renderable.get(), std::move(materialDict));

		registry.emplace<ecs::Renderable>(entity, std::move(renderable));
	}
}

void RenderableSystem::update(entt::entity entity, std::function<void(filament::MaterialInstance*)>&& callback)
{
	update(entity, "DefaultMaterial", std::move(callback));
}

void RenderableSystem::update(entt::entity entity, const std::string& name,
							  std::function<void(filament::MaterialInstance*)>&& callback)
{
	auto& materialInstance = mMaterialInstances.at(entity).at(name);
	callback(materialInstance.get());
}

} // namespace spatial::ecs