#pragma once

#include <utils/Entity.h>
#include <math/vec3.h>

namespace spatial::ecs
{

struct Transform
{
	filament::math::float3 position{.0f};
	filament::math::float3 scale{1.0f};
};

struct Renderable
{
	utils::Entity entity;
};

} // namespace spatial::ecs