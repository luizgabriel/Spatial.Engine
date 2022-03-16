#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

namespace tags
{

struct IsMainView
{
	constexpr static auto typeName = "tag_is_main_view";
};



} // namespace tags

struct SceneView
{
	constexpr static auto typeName = "scene_view";

	math::int2 size{1280, 720};
	Entity camera{ecs::NullEntity};
	Entity indirectLight{ecs::NullEntity};
};

} // namespace spatial::ecs