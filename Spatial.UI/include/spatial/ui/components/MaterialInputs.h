#pragma once

#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include "Icons.h"

namespace spatial::ui
{

void inputHelp(std::string_view title, std::string_view description);

void previewTexture(const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv = {0, 0, 1, 1});

bool colorPicker(std::string_view label, math::float4& color, const filament::Texture& icons);

bool colorPicker(std::string_view label, math::float3& color, const filament::Texture& icons);

bool cubemapInput(std::string_view label, Resource<CubeMapTexture>& resource, const filament::Texture& icons);

bool albedoInput(std::string_view label, math::float3& color, Resource<ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons);

bool mapInput(std::string_view label, Resource<ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv = Icons::bwPicture.uv());

bool mapInput(std::string_view label, float& value, Resource<ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv = Icons::bwPicture.uv());

}