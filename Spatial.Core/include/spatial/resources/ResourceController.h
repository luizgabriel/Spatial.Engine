#pragma once

#include <spatial/resources/FileSystem.h>
#include <spatial/ecs/Registry.h>

namespace spatial
{

struct ResourceController
{
	static void loadResources(FileSystem& fileSystem, ecs::Registry& registry);
};

} // namespace spatial::ecs