#include <boost/algorithm/string/predicate.hpp>
#include <fmt/format.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <sstream>

namespace spatial
{

std::unique_ptr<std::istream> MemoryFileSystem::openReadStreamImpl(std::string_view path) noexcept
{
	return std::make_unique<std::stringstream>(openStream(path));
}

std::unique_ptr<std::ostream> MemoryFileSystem::openWriteStreamImpl(std::string_view path) noexcept
{
	// TODO: Use span-stream on C++23
	auto stream = std::make_unique<std::stringstream>();
	stream->setstate(std::ios::failbit);
	return stream;
}

std::set<FileSystem::Entry> MemoryFileSystem::listImpl(std::string_view path) const
{
	using namespace boost::algorithm;

	auto result = std::set<Entry>();

	for (auto& [k, _] : mMemoryMap)
	{
		if (path.empty() || starts_with(k, path))
		{
			auto entryPath = k.substr(path.length());
			if (entryPath[0] == SEPARATOR)
				entryPath = entryPath.substr(1);

			auto separatorPos = entryPath.find(SEPARATOR);

			if (separatorPos == std::string_view::npos)
			{
				result.emplace(entryPath, FileType::File);
			}
			else
			{
				result.emplace(entryPath.substr(0, separatorPos), FileType::Directory);
			}
		}
	}

	return result;
}

void MemoryFileSystem::define(const std::string& fileName, std::string&& value)
{
	mMemoryMap.emplace(fileName, std::move(value));
}

void MemoryFileSystem::define(const std::string& fileName, const std::pair<const uint8_t*, size_t>& blob)
{
	auto value = std::string{};
	value.reserve(blob.second);
	std::copy(blob.first, blob.first + blob.second, std::back_inserter(value));

	define(fileName, std::move(value));
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

} // namespace spatial