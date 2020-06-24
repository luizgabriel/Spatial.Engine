#pragma once

#include <filament/MaterialInstance.h>
#include <math/vec4.h>

namespace spatial::ecs
{

struct DebugMesh
{
	filament::math::float4 color{1.0f, 1.0f, 1.0f, 1.0f};
	float metallic{.0f};
	float roughness{.0f};
	float clearCoat{.0f};
	float clearCoatRoughness{.0f};
};


} // namespace spatial::ecs