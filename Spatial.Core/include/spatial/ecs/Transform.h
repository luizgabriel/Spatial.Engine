#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct Transform
{
	math::float3 position{.0f};
	math::float3 scale{1.0f};
	math::float3 rotation{.0f};

	math::mat4f getMatrix() const noexcept;
};

}