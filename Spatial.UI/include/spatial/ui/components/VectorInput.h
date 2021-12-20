#pragma once

#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

bool buttonInput(std::string_view label, float& value, float resetValue = .0f, float speed = .1f, float min = .0f,
				 float max = .0f, std::string_view format = "%.2f");

bool vec2Input(std::string_view label, math::float2& v, float resetValue = .0f, float speed = .1f, float min = .0f,
			   float max = .0f, std::string_view format = "%.2f");

bool vec3Input(std::string_view label, math::float3& v, float resetValue = .0f, float speed = .1f, float min = .0f,
			   float max = .0f, std::string_view format = "%.2f");

bool vec4Input(std::string_view label, math::float4& v, float resetValue = .0f, float speed = .1f, float min = .0f,
			   float max = .0f, std::string_view format = "%.2f");

} // namespace spatial::ui
