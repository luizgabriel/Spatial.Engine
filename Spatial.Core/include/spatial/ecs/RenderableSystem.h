#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <filament/Engine.h>
#include <filament/RenderableManager.h>
#include <filament/Scene.h>
#include <unordered_map>
#include <utils/Entity.h>
#include <utils/EntityManager.h>

namespace spatial::ecs
{
class RenderableSystem
{
  private:
	filament::Scene& mScene;

  public:
	RenderableSystem(filament::Engine& engine, filament::Scene& scene);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onDestroy(entt::registry& registry, entt::entity entity);
};
} // namespace spatial::ecs
