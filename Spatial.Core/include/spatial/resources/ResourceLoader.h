#pragma once

#include <filesystem>
#include <string>
#include <tl/expected.hpp>
#include <vector>

namespace spatial
{

enum class ResourceError
{
	UnauthorizedPath,
	NotFound,
	NotAFile,
	OpenFailed,
	ParseError
};

tl::expected<std::filesystem::path, ResourceError> makeAbsolutePath(const std::filesystem::path& rootPath,
																	const std::filesystem::path& resourcePath);

tl::expected<std::filesystem::path, ResourceError> validateResourcePath(std::filesystem::path&& resourceAbsolutePath);

tl::expected<std::ifstream, ResourceError> openFileReadStream(const std::filesystem::path& resourceAbsolutePath);

tl::expected<std::ofstream, ResourceError> openFileWriteStream(const std::filesystem::path& resourceAbsolutePath);

std::vector<uint8_t> toVectorData(std::istream&& stream);

std::string toErrorMessage(ResourceError code);

ResourceError logResourceError(ResourceError code);

} // namespace spatial