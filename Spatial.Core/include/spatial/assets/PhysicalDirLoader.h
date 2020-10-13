#pragma once

#include <spatial/assets/ResourcesLoader.h>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace spatial::assets
{

class PhysicalDirLoader : public ResourcesLoader
{
  public:
	PhysicalDirLoader(std::filesystem::path basePath);

	assets::ResourceData load(const std::string_view fileName) const override;

  private:
	std::filesystem::path mRootPath;
};

} // namespace spatial::assets