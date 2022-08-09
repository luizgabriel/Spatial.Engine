#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/resources/FileSystem.h>

namespace spatial
{

struct ResourceController
{
	static void loadResources(FileSystem& fileSystem, ecs::Registry& registry);
};

} // namespace spatial