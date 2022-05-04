#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

struct LightController
{
	static void createLights(filament::Engine& engine, ecs::Registry& registry);
	static void updateLights(ecs::Registry& registry);
	static void deleteLights(ecs::Registry& registry);
};

} // namespace spatial::render