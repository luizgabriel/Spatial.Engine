#include <spatial/ecs/Components.h>
#include <spatial/ecs/RenderableSystem.h>
#include <execution>

namespace spatial::ecs
{

RenderableSystem::RenderableSystem(filament::Engine& engine, filament::Scene& scene)
	: m_manager{engine.getRenderableManager()}, m_scene{scene}, m_renderables{}
{
}

void RenderableSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	if (!m_renderables.contains(entity))
	{
		auto component = registry.get<Renderable>(entity);
		m_renderables.emplace(entity, component.entity);
		m_scene.addEntity(component.entity);
	}
}

void RenderableSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<Renderable>();

	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [this, &registry](auto entity) {
		auto component = registry.get<Renderable>(entity);
		auto renderableEntity = m_manager.getInstance(component.entity);

		m_manager.setCastShadows(renderableEntity, component.castShadows);
	});
}

void RenderableSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	if (m_renderables.contains(entity))
	{
		auto component = registry.get<Renderable>(entity);

		m_renderables.erase(entity);
		m_scene.remove(component.entity);
		utils::EntityManager::get().destroy(component.entity);
	}
}

RenderableSystem::~RenderableSystem()
{
	for (auto [entity, renderable] : m_renderables)
	{
		m_scene.remove(renderable);
		utils::EntityManager::get().destroy(renderable);
	}
}

} // namespace spatial::ecs