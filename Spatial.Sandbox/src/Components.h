#pragma once

#include <utils/Entity.h>
#include <filament/MaterialInstance.h>

namespace spatial::ecs
{

struct DebugCube
{
	float metallic;
	float roughness;
	float clearCoat;
	float clearCoatRoughness;
};


} // namespace spatial::ecs