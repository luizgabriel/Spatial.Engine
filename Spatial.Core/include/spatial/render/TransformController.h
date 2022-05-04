#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

struct TransformController
{
	static void createTransforms(filament::Engine& engine, ecs::Registry& registry);
	static void updateTransforms(ecs::Registry& registry);
};

} // namespace spatial::render