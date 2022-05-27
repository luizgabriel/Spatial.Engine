#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::graphics
{

struct TextureController
{
	static void loadTextures(filament::Engine& engine, ecs::Registry& registry);
};

} // namespace spatial::graphics