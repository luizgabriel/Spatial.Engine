#pragma once

#include <filament/MaterialInstance.h>
#include <math/vec4.h>

namespace spatial::ecs
{

struct DebugMesh
{
	filament::math::float4 color;
	float metallic;
	float roughness;
	float clearCoat;
	float clearCoatRoughness;
};


} // namespace spatial::ecs