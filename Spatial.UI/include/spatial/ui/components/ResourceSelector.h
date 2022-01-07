#pragma once

#include <imgui.h>
#include <spatial/render/ResourceFinders.h>
#include <spatial/resources/Resource.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>
#include <functional>

namespace spatial::ui
{

void imageTextureInput(std::string_view label, Resource<ResourceType::ImageTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, std::string_view dndTargetId = AssetsExplorer::DND_SELECTED_FILE);

void cubemapTextureInput(std::string_view label, Resource<ResourceType::CubeMapTexture>& resource, const filament::Texture* texture, const filament::Texture& icons, std::string_view dndTargetId = AssetsExplorer::DND_SELECTED_FILE);

} // namespace spatial::ui