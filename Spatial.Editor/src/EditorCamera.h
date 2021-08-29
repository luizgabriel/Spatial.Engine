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
	float sensitivity{1.0f};
	float velocity{1.0f};
	bool enabled{false};
	int justStarted{0};
	bool startPressed{false};
};

} // namespace spatial::editor