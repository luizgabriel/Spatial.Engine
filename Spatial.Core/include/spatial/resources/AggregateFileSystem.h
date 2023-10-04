#pragma once

#include <spatial/resources/FileSystem.h>
#include <unordered_map>

namespace spatial {

class AggregateFileSystem : public FileSystem
{
  public:
	using FileSystemMap = std::unordered_map<std::string, SharedFileSystem>;

	AggregateFileSystem() = default;

	AggregateFileSystem(std::initializer_list<FileSystemMap::value_type> fileSystems);

	void mount(std::string_view key, const SharedFileSystem& child);

	[[nodiscard]] std::unique_ptr<std::istream> openReadStream(std::string_view path) noexcept override;

	[[nodiscard]] std::unique_ptr<std::ostream> openWriteStream(std::string_view path) noexcept override;

	[[nodiscard]] std::set<Entry> list(std::string_view path) const override;

  private:
	FileSystemMap mFileSystems{};
};

}