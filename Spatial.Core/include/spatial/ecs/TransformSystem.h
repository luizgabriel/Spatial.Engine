#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <filament/Engine.h>
#include <filament/TransformManager.h>

namespace spatial::ecs
{

class TransformSystem
{
  private:
	filament::TransformManager& m_transformManager;

  public:
	explicit TransformSystem(filament::Engine& engine);

	void onUpdate(entt::registry& registry);
};

} // namespace spatial::ecs
