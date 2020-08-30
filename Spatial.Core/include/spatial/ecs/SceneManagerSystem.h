#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/MaterialRegistry.h>
#include <spatial/ecs/MeshRegistry.h>
#include <unordered_map>

namespace spatial::ecs
{

class SceneManagerSystem
{
  public:
	SceneManagerSystem(filament::Engine& engine, filament::Scene& scene);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onDestroy(entt::registry& registry, entt::entity entity);

  private:
	filament::Engine& mEngine;
	filament::Scene& mScene;
};

} // namespace spatial::ecs
