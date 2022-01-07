#pragma once

#include <filesystem>
#include <string>
#include <tl/expected.hpp>
#include <vector>
#include <fstream>
#include <filament/Engine.h>
#include <spatial/resources/ResourceError.h>
#include <spatial/resources/FilameshFile.h>
#include <spatial/resources/Resource.h>

namespace spatial
{

tl::expected<std::filesystem::path, ResourceError> makeAbsolutePath(const std::filesystem::path& rootPath,
																	const std::filesystem::path& resourcePath);

tl::expected<std::filesystem::path, ResourceError> validateResourcePath(std::filesystem::path&& resourceAbsolutePath);

tl::expected<std::filesystem::path, ResourceError> validateExtensions(std::filesystem::path&& resourceAbsolutePath, const std::vector<std::string>& extensions);

tl::expected<std::ifstream, ResourceError> openFileReadStream(const std::filesystem::path& resourceAbsolutePath);

tl::expected<std::ofstream, ResourceError> openFileWriteStream(const std::filesystem::path& resourceAbsolutePath);

std::vector<uint8_t> toVectorData(std::istream&& stream);

std::string toString(std::istream&& istream);

tl::expected<FilameshFile, ResourceError> toFilamesh(std::istream&& istream);

std::string toErrorMessage(ResourceError code);

ResourceError logResourceError(ResourceError code);

} // namespace spatial