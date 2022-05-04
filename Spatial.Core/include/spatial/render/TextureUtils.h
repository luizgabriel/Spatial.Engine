#pragma once

#include <filament/Texture.h>
#include <spatial/ecs/Registry.h>
#include <string_view>

namespace spatial::render
{

const filament::Texture* getTexture(const ecs::Registry& registry, ecs::Entity entity);

const filament::Texture* getTexture(const ecs::Registry& registry, std::string_view resourcePath);

}