#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

class SceneView
{
  public:
	static void image(const ecs::Registry& registry, ecs::Entity sceneViewEntity, math::float2 size);
};

} // namespace spatial::ui