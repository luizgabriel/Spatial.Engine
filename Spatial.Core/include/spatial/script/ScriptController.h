#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/script/Isolate.h>

namespace spatial::script
{

struct ScriptController {
	static void loadScripts(ecs::Registry& registry, FileSystem& fileSystem, Isolate& isolate);
};

} // namespace spatial::script