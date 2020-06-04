#include <spatial/ecs/Components.h>
#include <spatial/ecs/RenderableSystem.h>

namespace spatial::ecs
{

RenderableSystem::RenderableSystem(filament::Scene* scene)
	: m_scene{scene},
	  m_renderables{}
{
}

void RenderableSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	if (!m_renderables.contains(entity))
	{
		auto component = registry.get<Renderable>(entity);
		m_renderables.emplace(entity, component.entity);
		m_scene->addEntity(component.entity);
	}
}

void RenderableSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	if (m_renderables.contains(entity))
	{
		auto component = registry.get<Renderable>(entity);

		m_renderables.erase(entity);
		m_scene->remove(component.entity);
		utils::EntityManager::get().destroy(component.entity);
	}
}

RenderableSystem::~RenderableSystem()
{
	for (auto [entity, renderable] : m_renderables)
	{
		m_scene->remove(renderable);
		utils::EntityManager::get().destroy(renderable);
	}
}

}