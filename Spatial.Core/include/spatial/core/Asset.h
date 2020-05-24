#pragma once

#include <filesystem>
#include <spatial/common/Exceptions.h>

namespace spatial
{

class Asset
{
private:
	static std::filesystem::path s_basePath;

public:
	static void init(const std::filesystem::path& basePath) { s_basePath = basePath; }

	static std::filesystem::path absolute(const std::filesystem::path& relative) {
		auto absolutePath = s_basePath / relative;

		if (!std::filesystem::exists(absolutePath))
			throw FileNotFoundError(absolutePath);

		return absolutePath;
	}
};

} // namespace spatial