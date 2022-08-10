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

OptionalTexture getTexture(const ecs::Registry& registry, std::string_view resourcePath);

} // namespace spatial::graphics