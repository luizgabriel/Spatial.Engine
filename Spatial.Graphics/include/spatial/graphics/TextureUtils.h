#pragma once

#include <optional>
#include <spatial/ecs/Registry.h>
#include <spatial/graphics/Resources.h>
#include <string_view>
#include <memory>

namespace spatial::graphics
{

using OptionalTexture = std::optional<SharedTexture>;

OptionalTexture getTexture(const ecs::Registry& registry, ecs::Entity entity);

template <typename ResourceTag>
OptionalTexture getTexture(const ecs::Registry& registry)
{
	auto entity = registry.getFirstEntity<ResourceTag>();
	if (!registry.isValid(entity))
		return std::nullopt;

	return getTexture(registry, entity);
}

} // namespace spatial::graphics