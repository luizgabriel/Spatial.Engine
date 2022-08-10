#pragma once

#include "Icons.h"
#include <filament/Texture.h>
#include <spatial/common/Math.h>

namespace spatial::ui
{

bool colorPicker(std::string_view label, math::vec4& color, graphics::OptionalTexture icons);

bool colorPicker(std::string_view label, math::vec3& color, graphics::OptionalTexture icons);

bool cubemapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource,
				  graphics::OptionalTexture icons);

bool albedoInput(std::string_view label, math::vec3& color, ecs::Registry& registry, ecs::Entity& resource,
				 graphics::OptionalTexture icons);

bool mapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource, graphics::OptionalTexture icons,
			  math::vec4 uv = Icons::bwPicture.uv());

bool mapInput(std::string_view label, float& value, ecs::Registry& registry, ecs::Entity& resource,
			  graphics::OptionalTexture icons, math::vec4 uv = Icons::bwPicture.uv());

} // namespace spatial::ui