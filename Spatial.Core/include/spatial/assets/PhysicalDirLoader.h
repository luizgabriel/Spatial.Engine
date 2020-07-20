#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

namespace spatial
{

class PhysicalDirLoader
{
  public:
	PhysicalDirLoader(std::filesystem::path basePath)
		: mRootPath{std::move(basePath)}
	{
	}

	bool operator()(const std::string_view fileName, auto buffer) const noexcept
	{
		auto fs = std::fstream{mRootPath / fileName};
		if (!fs)
			return false;

		std::copy(std::istream_iterator<char>(fs), std::istream_iterator<char>(), buffer);

		return true;
	}

  private:
	std::filesystem::path mRootPath;
};

} // namespace spatial