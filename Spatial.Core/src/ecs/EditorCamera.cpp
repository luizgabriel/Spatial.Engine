#include <spatial/ecs/Transform.h>

namespace spatial::ecs
{

Transform::Transform(const math::float3& position, const math::float3& scale, const math::float3& rotation)
	: position(position), scale(scale), rotation(rotation)
{
}

math::mat4f Transform::getMatrix() const noexcept
{
	const auto tm = math::mat4f::translation(position);
	const auto rm = math::mat4f::eulerZYX(rotation.z, rotation.y, rotation.x);
	const auto sm = math::mat4f::scaling(scale);

	return tm * rm * sm;
}

}