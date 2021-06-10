#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct SceneNode
{
	Entity parent{NullEntity};
	Entity next{NullEntity};
};

}
