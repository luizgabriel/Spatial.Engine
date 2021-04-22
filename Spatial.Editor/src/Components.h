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
	float sensitivity;
	float velocity;
	bool enabled{false};
	int justStarted{0};
	bool startPressed{false};
};

} // namespace spatial::ecs