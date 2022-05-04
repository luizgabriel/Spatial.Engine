#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/resources/FileSystem.h>

namespace spatial::render
{

struct TextureController
{
	static void loadTextures(filament::Engine& engine, FileSystem& fileSystem, ecs::Registry& registry);
};

} // namespace spatial::render