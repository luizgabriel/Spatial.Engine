#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/MaterialSystem.h>
#include <spatial/ecs/MeshSystem.h>
#include <unordered_map>

namespace spatial::ecs
{

class RenderableSystem
{
  public:
	RenderableSystem(filament::Engine& engine, filament::Scene& scene);

	void buildMaterialInstances(const entt::registry& registry, const ecs::MaterialSystem& materialSystem);

	void buildShapeRenderables(entt::registry& registry, ecs::MeshSystem& meshSystem);

	void update(entt::entity entity, std::function<void(filament::MaterialInstance*)>&& callback);

	void update(entt::entity entity, const std::string& name, std::function<void(filament::MaterialInstance*)>&& callback);

	void onUpdate(entt::registry& registry);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onDestroy(entt::registry& registry, entt::entity entity);

  private:
	filament::Engine& mEngine;
	filament::Scene& mScene;
	filament::RenderableManager& mRenderableManager;

	std::unordered_map<entt::entity, std::unordered_map<std::string, MaterialInstance>> mMaterialInstances;
};

} // namespace spatial::ecs
