#pragma once

#include <spatial/resources/FileSystem.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct ResourceController
{
	static void loadResources(FileSystem& fileSystem, ecs::Registry& registry);
};

} // namespace spatial::ecs