#pragma once

#include <entt/entity/registry.hpp>
#include <spatial/ecs/MaterialRegistry.h>
#include <spatial/ecs/MeshRegistry.h>

namespace spatial::ecs
{

class RenderableSystem
{
  private:
	MaterialRegistry& mMaterialRegistry;
	MeshRegistry& mMeshRegistry;
	filament::RenderableManager& mRenderableManager;

  public:
	RenderableSystem(MaterialRegistry& materialRegistry, MeshRegistry& meshRegistry);

	void onConstruct(entt::registry& registry, entt::entity entity);

	void onDestroy(entt::registry& registry, entt::entity entity);

	void onUpdate(entt::registry& registry);
};

} // namespace spatial::ecs