#include <spatial/ecs/Transform.h>

namespace spatial::ecs
{

math::mat4 Transform::getTranslationMatrix() const noexcept
{
	return math::translate(position);
}

math::mat4 Transform::getScaleMatrix() const noexcept
{
	return math::scale(scale);
}

math::mat4 Transform::getRotationMatrix() const noexcept
{
	return math::rotate(rotation.z, math::axisZ) * math::rotate(rotation.y, math::axisY)
		   * math::rotate(rotation.x, math::axisX);
}

math::mat4 Transform::getMatrix() const noexcept
{
	return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
}

math::vec3 Transform::getForwardVector() const noexcept
{
	return math::forwardVector(getMatrix());
}

math::vec3 Transform::getEulerAnglesRotation() const noexcept
{
	return rotation * math::rad2deg;
}

} // namespace spatial::ecs