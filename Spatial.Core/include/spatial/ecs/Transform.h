#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct Transform
{
	math::float3 position;
	math::float3 scale;
	math::float3 rotation;

	explicit Transform(const math::float3& position = {0}, const math::float3& scale = {1}, const math::float3& rotation = {0});

	[[nodiscard]] math::mat4f getMatrix() const noexcept;
};

} // namespace spatial::ecs