#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

namespace tags
{

struct IsMainView
{
};

struct IsRenderable
{
};

struct CanDestroy
{
};

} // namespace tags

struct SceneView
{
	math::int2 size{1280, 720};
	Entity camera{ecs::NullEntity};
	Entity indirectLight{ecs::NullEntity};
};

} // namespace spatial::ecs