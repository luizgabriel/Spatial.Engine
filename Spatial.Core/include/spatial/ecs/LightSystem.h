#pragma once

#include <filament/Engine.h>
#include <filament/LightManager.h>
#include <entt/entity/registry.hpp>
#include <unordered_map>
#include <spatial/render/Resources.h>

namespace spatial::ecs
{

class LightSystem
{
  public:
	LightSystem(filament::Engine& engine);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onDestruct(entt::registry& registry, entt::entity entity);

	void onUpdate(entt::registry& registry);

  private:
	filament::Engine& mEngine;
	filament::LightManager& mLightManager;
};

}
