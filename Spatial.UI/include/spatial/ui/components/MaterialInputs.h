#pragma once

#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include "Icons.h"

namespace spatial::ui
{

void previewTexture(const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv = {0, 0, 1, 1});

bool cubemapInput(std::string_view label, math::float4& color, Resource<CubeMapTexture>& resource, const filament::Texture& icons, std::string_view dndTargetId = AssetsExplorer::DND_SELECTED_FILE);

bool albedoInput(std::string_view label, math::float3& color, Resource<ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, std::string_view dndTargetId = AssetsExplorer::DND_SELECTED_FILE);

bool mapInput(std::string_view label, Resource<ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv = Icons::bwPicture.uv(), std::string_view dndTargetId = AssetsExplorer::DND_SELECTED_FILE);

bool mapInput(std::string_view label, float& value, Resource<ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, const math::float4& uv = Icons::bwPicture.uv(), std::string_view dndTargetId = AssetsExplorer::DND_SELECTED_FILE);

}