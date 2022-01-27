#include <fmt/format.h>
#include <spatial/resources/FilesSystem.h>

namespace spatial
{

void FileSystem::mount(std::string_view key, const SharedFileSystem& child)
{
	auto result = resolve(key);

	assert(result.path.find(SEPARATOR) == std::string_view::npos);
	result.fileSystem.mNodes.emplace(result.path, child);
}

FileSystem::ResolveResult FileSystem::resolve(std::string_view path) noexcept
{
	auto result = FileSystem::ResolveResult{std::string{path}, *this};

	auto separatorPos = path.find(SEPARATOR);
	if (separatorPos == std::string_view::npos)
	{
		if (!path.empty())
		{
			auto it = mNodes.find(result.path);
			if (it != mNodes.end())
				return ResolveResult{"", *it->second};
		}

		return result;
	}

	auto directoryName = std::string{path.substr(0, separatorPos)};

	auto it = mNodes.find(directoryName);
	if (it == mNodes.end())
		return result;

	auto restOfPath = path.substr(separatorPos + 1);
	return it->second->resolve(restOfPath);
}

std::unique_ptr<std::istream> FileSystem::openReadStream(std::string_view path)
{
	auto result = resolve(path);
	return result.fileSystem.openReadStreamImpl(result.path);
}

std::string FileSystem::readString(std::string_view path)
{
	auto stream = openReadStream(path);
	auto value = std::string{};
	std::copy(std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>(), std::back_inserter(value));
	return value;
}

std::vector<uint8_t> FileSystem::readBinary(std::string_view path)
{
	auto stream = openReadStream(path);
	auto value = std::vector<uint8_t>{};
	std::copy(std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>(), std::back_inserter(value));
	return value;
}

std::unique_ptr<std::ostream> FileSystem::openWriteStream(std::string_view path)
{
	auto result = resolve(path);
	return result.fileSystem.openWriteStreamImpl(result.path);
}

void FileSystem::write(std::string_view path, std::string&& data)
{
	auto stream = openWriteStream(path);
	if (stream->fail())
		throw std::invalid_argument{fmt::format("Could not open file: {}", path)};

	std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(*stream));
}

std::set<FileSystem::Entry> FileSystem::list(std::string_view path)
{
	auto result = resolve(path);
	auto pathList = result.fileSystem.listImpl(result.path);

	for (auto& [k, _] : result.fileSystem.mNodes)
		pathList.emplace(k, FileType::Directory);

	return pathList;
}

std::unique_ptr<std::istream> RootFileSystem::openReadStreamImpl(std::string_view path)
{
	throw makeException(path);
}

std::unique_ptr<std::ostream> RootFileSystem::openWriteStreamImpl(std::string_view path)
{
	throw makeException(path);
}

std::set<FileSystem::Entry> RootFileSystem::listImpl(std::string_view path) const
{
	return std::set<Entry>{};
}

std::invalid_argument RootFileSystem::makeException(const std::string_view& path)
{
	return std::invalid_argument{fmt::format("Invalid path: {}", path)};
}

} // namespace spatial