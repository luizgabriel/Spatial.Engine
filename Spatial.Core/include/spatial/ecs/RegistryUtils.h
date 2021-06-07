#pragma once

#include <boost/tti/has_member_function.hpp>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

EntityHandle createEntity(Registry& registry);

EntityHandle createEntity(Registry& registry, std::string name);

} // namespace spatial