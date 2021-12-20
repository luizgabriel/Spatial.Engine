#pragma once

#include "Window.h"
#include <filament/Texture.h>
#include <filesystem>
#include <opencl-c-base.h>
#include <spatial/common/Math.h>
#include <spatial/common/StringHelpers.h>
#include <unordered_map>

namespace spatial::ui
{

class AssetsExplorer
{
  public:
	static constexpr auto DND_SELECTED_FILE = "SELECTED_ASSET_PATH";

	static bool displayFiles(const std::filesystem::path& rootPath, std::filesystem::path& selectedPath,
							 const filament::Texture* icon = nullptr);

  private:
	ui::Window mWindow;
	static filament::Texture* sIconTexture;

	static bool displayPathHeader(const std::filesystem::path& rootPath, std::filesystem::path& selectedPath,
								  const filament::Texture* icon = nullptr);
};

} // namespace spatial::ui