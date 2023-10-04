#pragma once

#include <spatial/resources/FileSystem.h>
#include <unordered_map>

namespace spatial
{

class MemoryFileSystem : public FileSystem
{
  public:
	using FilesMap = std::unordered_map<std::string, std::string>;

	MemoryFileSystem() = default;

	MemoryFileSystem(std::initializer_list<FilesMap::value_type> files);

	void define(const std::string& fileName, std::string&& value);

	void define(const std::string& fileName, const std::pair<const uint8_t*, size_t>& blob);

	[[nodiscard]] std::unique_ptr<std::istream> openReadStream(std::string_view path) noexcept override;

	[[nodiscard]] std::unique_ptr<std::ostream> openWriteStream(std::string_view path) noexcept override;

	[[nodiscard]] std::set<Entry> list(std::string_view path) const override;

  private:
	std::unordered_map<std::string, std::string> mMemoryMap;

	[[nodiscard]] std::stringstream openStream(std::string_view path) const;
};

std::string toString(const std::pair<const uint8_t*, size_t> blob);

} // namespace spatial
