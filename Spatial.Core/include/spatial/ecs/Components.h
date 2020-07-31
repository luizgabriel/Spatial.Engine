#pragma once

#include <math/vec3.h>
#include <spatial/render/Entity.h>

namespace spatial::ecs
{

struct Transform
{
	filament::math::float3 position{.0f};
	filament::math::float3 scale{1.0f};
};

struct Renderable
{
	Entity entity;
};

} // namespace spatial::ecs