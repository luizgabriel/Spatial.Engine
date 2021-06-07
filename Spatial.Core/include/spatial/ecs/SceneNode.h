#pragma once

#include <string>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct SceneNode
{
	std::string name;
	Entity parent{NullEntity};
	Entity next{NullEntity};
};

} // namespace spatial::ecs