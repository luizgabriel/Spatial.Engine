#pragma once

#include <filament/Texture.h>
#include <spatial/resources/FileSystem.h>

namespace spatial::ui
{

class FilesExplorer
{
  public:
	static bool displayFiles(FileSystem& fileSystem, std::string& selectedPath, const filament::Texture* icon);

  private:
	static bool displayPathHeader(std::string& selectedPath, const filament::Texture* icon);
};

} // namespace spatial::ui