#pragma once

#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/Registry.h>
#include <optional>

namespace spatial::ecs
{

EntityHandle createEntity(Registry& registry);

EntityHandle createEntity(Registry& registry, std::string name);

EntityBuilder build(Registry& registry);

Entity findEntityFromName(const ecs::Registry& registry, std::string_view entityName);

Entity findEntityFromUUID(const ecs::Registry& registry, UUID uuid);

std::optional<UUID> findUUIDFromEntityName(const ecs::Registry& registry, std::string_view name);


} // namespace spatial