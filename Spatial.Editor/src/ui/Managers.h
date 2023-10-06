#pragma once

#include "Components.h"
#include <spatial/ecs/Registry.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::ui
{

struct RegistryManager
{
	static void list(ecs::Registry& registry, std::string_view search, ComponentFilter componentFilter, bool showEditorEntities);
};

} // namespace spatial::ui