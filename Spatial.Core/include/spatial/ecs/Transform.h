#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct Transform
{
	math::float3 position;
	math::float3 scale;
	math::float3 rotation;

	Transform() : position{0}, scale{1}, rotation{0}
	{
	}

	Transform(const math::float3& position, const math::float3& scale, const math::float3& rotation);

	[[nodiscard]] math::mat4f getMatrix() const noexcept;
};

}