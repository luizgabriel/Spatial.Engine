#include <cassert>
#include <fmt/format.h>
#include <spatial/resources/FileSystem.h>
#include <sstream>

namespace spatial
{

std::string FileSystem::readString(std::string_view path) noexcept
{
	auto stream = openReadStream(path);
	if (stream->fail())
		return "";

	return std::string{std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>()};
}

std::vector<uint8_t> FileSystem::readBinary(std::string_view path) noexcept
{
	auto stream = openReadStream(path);
	if (stream->fail())
		return std::vector<uint8_t>{};

	return std::vector<uint8_t>{std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>()};
}

bool FileSystem::write(std::string_view path, std::string&& data) noexcept
{
	auto stream = openWriteStream(path);
	if (stream->fail())
		return false;

	std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(*stream));
	return true;
}

std::future<std::vector<uint8_t>> FileSystem::readBinaryAsync(std::string_view path) noexcept
{
	return std::async(std::launch::async,
					  [this, p = std::string{path}]() -> std::vector<uint8_t> { return this->readBinary(p); });
}

std::pair<std::string_view, std::string_view> splitPath(std::string_view path) noexcept
{
	auto pos = path.find_first_of(FileSystem::SEPARATOR);
	if (pos == std::string_view::npos)
		return {path, ""};

	return {path.substr(0, pos), path.substr(pos + 1)};
}

} // namespace spatial