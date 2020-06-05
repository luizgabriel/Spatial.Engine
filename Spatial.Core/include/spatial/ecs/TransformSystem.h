#pragma once

#include <filament/TransformManager.h>
#include <filament/Engine.h>
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>

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

}
