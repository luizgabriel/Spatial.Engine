#pragma once

#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ui
{

void previewTexture(const ecs::Registry& registry, ecs::Entity resourceEntity, graphics::OptionalTexture icons,
					const math::vec4 uv = {0, 0, 1, 1});

}