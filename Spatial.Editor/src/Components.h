#pragma once

#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <string>
#include <unordered_map>
#include <variant>

namespace spatial::ecs
{

struct DefaultMaterial {
	filament::math::float4 baseColor;

};

} // namespace spatial::ecs