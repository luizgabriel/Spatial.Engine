#pragma once

#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

bool directionInput(std::string_view label, math::float3& v, float size = 100.0f, std::uint32_t color = 0x22ff2200);

}