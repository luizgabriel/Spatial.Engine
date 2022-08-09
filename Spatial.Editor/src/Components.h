#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::editor
{

struct EditorCamera
{
	constexpr static auto typeName = "editor_camera";

	float sensitivity{.5F};
	float velocity{10.0F};

	math::vec2 viewOffset{};
	math::vec3 movementOffset{};

	static void replaceView(ecs::Registry& registry, ecs::Entity newControlledView);
	static void replaceCamera(ecs::Registry& registry, ecs::Entity newControlledEntity);
};

} // namespace spatial::editor