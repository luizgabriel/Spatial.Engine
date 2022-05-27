#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

struct SceneView
{
	static void image(const ecs::Registry& registry, ecs::Entity sceneViewEntity, math::vec2 size);
	static bool selector(const ecs::Registry& registry, ecs::Entity& sceneViewEntity);
};

} // namespace spatial::ui