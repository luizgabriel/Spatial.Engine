#include <spatial/ecs/Transform.h>

namespace spatial::ecs
{

math::mat4f Transform::getMatrix() const noexcept
{
	const auto tm = math::mat4f::translation(position);
	const auto rm = math::mat4f::eulerZYX(rotation.z, rotation.y, rotation.x);
	const auto sm = math::mat4f::scaling(scale);

	return tm * rm * sm;
}

}