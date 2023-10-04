#include <spatial/resources/AggregateFileSystem.h>
#include <sstream>

namespace spatial
{

AggregateFileSystem::AggregateFileSystem(
	std::initializer_list<AggregateFileSystem::FileSystemMap::value_type> fileSystems)
	: mFileSystems(fileSystems)
{
}

std::unique_ptr<std::iostream> makeFailedStream() noexcept
{
	auto stream = std::make_unique<std::stringstream>("");
	stream->setstate(std::ios::failbit);
	return stream;
}

std::unique_ptr<std::istream> AggregateFileSystem::openReadStream(std::string_view path) noexcept
{
	auto [folder, rest] = splitPath(path);
	auto it = mFileSystems.find(std::string{folder});
	if (it == mFileSystems.end())
		return makeFailedStream();

	return it->second->openReadStream(rest);
}

std::unique_ptr<std::ostream> AggregateFileSystem::openWriteStream(std::string_view path) noexcept
{
	auto [folder, rest] = splitPath(path);
	auto it = mFileSystems.find(std::string{folder});
	if (it == mFileSystems.end())
		return makeFailedStream();

	return it->second->openWriteStream(rest);
}

std::set<FileSystem::Entry> AggregateFileSystem::list(std::string_view path) const
{
	auto [folder, rest] = splitPath(path);
	if (folder.empty())
	{
		auto result = std::set<FileSystem::Entry>{};
		for (const auto& [key, fs] : mFileSystems)
		{
			auto entry = FileSystem::Entry{key, FileSystem::EntryType::Directory};
			result.insert(entry);
		}

		return result;
	}

	auto it = mFileSystems.find(std::string{folder});

	if (it == mFileSystems.end())
		return {};

	return it->second->list(rest);
}

void AggregateFileSystem::mount(std::string_view key, const SharedFileSystem& child)
{
	mFileSystems.emplace(key, child);
}

} // namespace spatial