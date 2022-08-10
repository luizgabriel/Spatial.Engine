#pragma once

#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

bool buttonInput(std::string_view label, uint32_t& value, uint32_t resetValue = 0, uint32_t speed = 1, uint32_t min = 0,
				 uint32_t max = 0);

bool buttonInput(std::string_view label, float& value, float resetValue = .0f, float speed = .1f, float min = .0f,
				 float max = .0f, std::string_view format = "%.2f");

bool vec2Input(std::string_view label, math::uvec2& v, uint32_t resetValue = 0, uint32_t speed = 1, uint32_t min = 0,
			   uint32_t max = 0);

bool vec2Input(std::string_view label, math::vec2& v, float resetValue = .0f, float speed = .1f, float min = .0f,
			   float max = .0f, std::string_view format = "%.2f");

bool vec3Input(std::string_view label, math::vec3& v, float resetValue = .0f, float speed = .1f, float min = .0f,
			   float max = .0f, std::string_view format = "%.2f");

bool vec4Input(std::string_view label, math::vec4& v, float resetValue = .0f, float speed = .1f, float min = .0f,
			   float max = .0f, std::string_view format = "%.2f");

} // namespace spatial::ui
