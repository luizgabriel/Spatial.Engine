#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct Transform
{
	static constexpr auto typeName = "transform";

	math::vec3 position;
	math::vec3 scale;
	math::vec3 rotation;

	explicit Transform(const math::vec3& position = math::vec3{0}, const math::vec3& scale = math::vec3{1},
								 const math::vec3& rotation = math::vec3{0})
		: position(position), scale(scale), rotation(rotation)
	{
	}

	[[nodiscard]] math::vec3 getEulerAnglesRotation() const noexcept;

	[[nodiscard]] math::mat4 getTranslationMatrix() const noexcept;

	[[nodiscard]] math::mat4 getScaleMatrix() const noexcept;

	[[nodiscard]] math::mat4 getRotationMatrix() const noexcept;

	[[nodiscard]] math::mat4 getMatrix() const noexcept;

	[[nodiscard]] math::vec3 getForwardVector() const noexcept;
};

} // namespace spatial::ecs