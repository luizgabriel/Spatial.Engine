#pragma once

#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Registry.h>
#include <string>

namespace spatial::ecs
{

EntityHandle createEntity(Registry& registry);

EntityHandle createEntity(Registry& registry, std::string name);

EntityBuilder build(Registry& registry);

} // namespace spatial::ecs