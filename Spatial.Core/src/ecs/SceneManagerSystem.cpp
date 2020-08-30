#include <spatial/ecs/Components.h>
#include <spatial/ecs/SceneManagerSystem.h>
#include <utils/EntityManager.h>

namespace spatial::ecs
{

SceneManagerSystem::SceneManagerSystem(filament::Engine& engine, filament::Scene& scene)
	: mEngine{engine}, mScene{scene}
{
}

void SceneManagerSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	auto& component = registry.get<ecs::SceneEntity>(entity);
	component.entity = utils::EntityManager::get().create();

	mScene.addEntity(component.entity);
}

void SceneManagerSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	auto& component = registry.get<ecs::SceneEntity>(entity);
	mScene.remove(component.entity);
	utils::EntityManager::get().destroy(component.entity);
}

} // namespace spatial::ecs