#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct Transform
{
	math::float3 position;
	math::float3 scale;
	math::float3 rotation;

	explicit constexpr Transform(const math::float3& position = {0}, const math::float3& scale = {1}, const math::float3& rotation = {0})
		: position(position), scale(scale), rotation(rotation)
	{
	}

	[[nodiscard]] math::float3 getEulerAnglesRotation() const noexcept;

	[[nodiscard]] math::mat4f getTranslationMatrix() const noexcept;

	[[nodiscard]] math::mat4f getScaleMatrix() const noexcept;

	[[nodiscard]] math::mat4f getRotationMatrix() const noexcept;

	[[nodiscard]] math::mat4f getMatrix() const noexcept;

	[[nodiscard]] math::float3 getForwardVector() const noexcept;
};

} // namespace spatial::ecs