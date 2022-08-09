#pragma once

#include "Icons.h"
#include <filament/Texture.h>
#include <spatial/common/Math.h>

namespace spatial::ui
{

bool colorPicker(std::string_view label, math::vec4& color, const filament::Texture* icons);

bool colorPicker(std::string_view label, math::vec3& color, const filament::Texture* icons);

bool cubemapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource,
				  const filament::Texture* icons);

bool albedoInput(std::string_view label, math::vec3& color, ecs::Registry& registry, ecs::Entity& resource,
				 const filament::Texture* icons);

bool mapInput(std::string_view label, ecs::Registry& registry, ecs::Entity& resource, const filament::Texture* icons,
			  math::vec4 uv = Icons::bwPicture.uv());

bool mapInput(std::string_view label, float& value, ecs::Registry& registry, ecs::Entity& resource,
			  const filament::Texture* icons, math::vec4 uv = Icons::bwPicture.uv());

} // namespace spatial::ui