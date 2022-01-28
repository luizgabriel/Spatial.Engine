#pragma once

#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <string>
#include <unordered_map>
#include <variant>

namespace spatial::editor
{

struct EditorCamera
{
	constexpr static auto typeName = "editor_camera";

	float sensitivity{1.0f};
	float velocity{1.0f};
};

} // namespace spatial::editor