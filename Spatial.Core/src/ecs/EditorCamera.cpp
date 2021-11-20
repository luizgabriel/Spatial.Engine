#include <spatial/ecs/Transform.h>

namespace spatial::ecs
{

math::mat4f Transform::getTranslationMatrix() const noexcept
{
	return math::mat4f::translation(position);
}

math::mat4f Transform::getScaleMatrix() const noexcept
{
	return math::mat4f::scaling(scale);
}

math::mat4f Transform::getRotationMatrix() const noexcept
{
	return math::mat4f::eulerZYX(rotation.z, rotation.y, rotation.x);
}

math::mat4f Transform::getMatrix() const noexcept
{
	return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
}

math::float3 Transform::getForwardVector() const noexcept
{
	return math::forwardVector(getMatrix());
}

math::float3 Transform::getEulerAnglesRotation() const noexcept
{
	return rotation * math::rad2deg;
}

}