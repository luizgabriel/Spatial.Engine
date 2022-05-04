#pragma once

#include <spatial/ui/components/Window.h>
#include <filament/Texture.h>
#include <filesystem>
#include <spatial/common/Math.h>
#include <spatial/resources/FileSystem.h>
#include <unordered_map>

namespace spatial::ui
{

class AssetsExplorer
{
  public:
	static bool displayFiles(FileSystem& fileSystem, std::filesystem::path& selectedPath,
							 const filament::Texture* icon);

  private:
	static bool displayPathHeader(std::filesystem::path& selectedPath, const filament::Texture* icon);
};

} // namespace spatial::ui