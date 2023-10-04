#include <fmt/format.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <sstream>

namespace spatial
{

MemoryFileSystem::MemoryFileSystem(std::initializer_list<MemoryFileSystem::FilesMap::value_type> files) : mMemoryMap{files}
{
}

std::unique_ptr<std::istream> MemoryFileSystem::openReadStream(std::string_view path) noexcept
{
	return std::make_unique<std::stringstream>(openStream(path));
}

std::unique_ptr<std::ostream> MemoryFileSystem::openWriteStream(std::string_view path) noexcept
{
	// TODO: Use span-stream on C++23
	auto stream = std::make_unique<std::stringstream>();
	stream->setstate(std::ios::failbit);
	return stream;
}

std::set<FileSystem::Entry> MemoryFileSystem::list(std::string_view path) const
{
	auto result = std::set<Entry>();

	for (auto& [k, _] : mMemoryMap)
	{
		result.emplace(k, EntryType::File);
	}

	return result;
}

void MemoryFileSystem::define(const std::string& fileName, std::string&& value)
{
	mMemoryMap.emplace(fileName, std::move(value));
}

void MemoryFileSystem::define(const std::string& fileName, const std::pair<const uint8_t*, size_t>& blob)
{
	define(fileName, toString(blob));
}

std::stringstream MemoryFileSystem::openStream(std::string_view path) const
{
	try
	{
		auto& data = mMemoryMap.at(std::string{path});
		return std::stringstream{data};
	}
	catch (const std::out_of_range& e)
	{
		auto value = std::stringstream{};
		value.setstate(std::ios::badbit);
		return value;
	}
}

std::string toString(const std::pair<const uint8_t*, size_t> blob)
{
	auto value = std::string{};
	value.reserve(blob.second);
	std::copy(blob.first, blob.first + blob.second, std::back_inserter(value));

	return value;
}

} // namespace spatial