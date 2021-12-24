#include <fmt/format.h>
#include <fstream>
#include <magic_enum.hpp>
#include <spatial/core/Logger.h>
#include <spatial/resources/ResourceLoader.h>

namespace spatial
{

tl::expected<std::filesystem::path, ResourceError> makeAbsolutePath(const std::filesystem::path& rootPath,
																	const std::filesystem::path& resourcePath)
{
	const auto finalPath = (rootPath / resourcePath).lexically_normal();

	auto [rootEnd, nothing] = std::mismatch(rootPath.begin(), rootPath.end(), finalPath.begin());

	if (rootEnd != rootPath.end())
		return tl::make_unexpected(ResourceError::UnauthorizedPath);

	return finalPath;
}

tl::expected<std::filesystem::path, ResourceError> validateResourcePath(std::filesystem::path&& resourceAbsolutePath)
{
	if (!std::filesystem::exists(resourceAbsolutePath))
		return tl::make_unexpected(ResourceError::NotFound);

	if (!std::filesystem::is_regular_file(resourceAbsolutePath))
		return tl::make_unexpected(ResourceError::NotAFile);

	return resourceAbsolutePath;
}

tl::expected<std::ifstream, ResourceError> openFileReadStream(const std::filesystem::path& resourceAbsolutePath)
{
	auto ifs = std::ifstream{resourceAbsolutePath};
	if (!ifs)
		return tl::make_unexpected(ResourceError::OpenFailed);

	return ifs;
}

tl::expected<std::ofstream, ResourceError> openFileWriteStream(const std::filesystem::path& resourceAbsolutePath)
{
	auto ofs = std::ofstream{resourceAbsolutePath};
	if (!ofs)
		return tl::make_unexpected(ResourceError::OpenFailed);

	return ofs;
}

std::vector<uint8_t> toVectorData(std::istream&& stream)
{
	auto result = std::vector<uint8_t>{};
	std::copy(std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{}, std::back_inserter(result));

	return result;
}

std::string toErrorMessage(ResourceError code)
{
	const auto name = magic_enum::enum_name(code);
	return fmt::format("Resource Error: {}", name);
}

ResourceError logResourceError(ResourceError code)
{
	static auto gLogger = createDefaultLogger();
	gLogger.error(toErrorMessage(code));

	return code;
}

} // namespace spatial