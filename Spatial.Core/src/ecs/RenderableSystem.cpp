#include <spatial/ecs/Components.h>
#include <spatial/ecs/RenderableSystem.h>

namespace spatial::ecs
{

RenderableSystem::RenderableSystem(filament::Engine& engine, filament::Scene& scene)
	: m_manager{engine.getRenderableManager()}, m_scene{scene}
{
}

void RenderableSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	auto& component = registry.get<Renderable>(entity);
	m_scene.addEntity(component.entity.get());
}

void RenderableSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	auto& component = registry.get<Renderable>(entity);
	m_scene.remove(component.entity.get());
}

} // namespace spatial::ecs