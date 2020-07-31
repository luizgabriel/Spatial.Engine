#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

namespace spatial
{

class PhysicalDirLoader
{
  public:
	PhysicalDirLoader(std::filesystem::path basePath) : mRootPath{std::move(basePath)}
	{
	}

	assets::Resource operator()(const std::string_view fileName) const noexcept
	{
		auto fs = std::fstream{mRootPath / fileName};
		if (!fs)
			return std::nullopt;

		return std::vector<char>{std::istreambuf_iterator<char>{fs}, std::istreambuf_iterator<char>{}};
	}

  private:
	std::filesystem::path mRootPath;
};

} // namespace spatial