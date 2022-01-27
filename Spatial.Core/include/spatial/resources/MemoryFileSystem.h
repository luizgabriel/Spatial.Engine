#pragma once

#include <spatial/resources/FilesSystem.h>
#include <unordered_map>

namespace spatial
{

class MemoryFileSystem : public FileSystem
{
  public:
	MemoryFileSystem() = default;

	void define(const std::string& fileName, std::string&& value);

	void define(const std::string& fileName, const std::pair<const uint8_t*, size_t>& blob);

  protected:

	[[nodiscard]] std::unique_ptr<std::istream> openReadStreamImpl(std::string_view path) override;

	[[nodiscard]] std::unique_ptr<std::ostream> openWriteStreamImpl(std::string_view path) override;

	[[nodiscard]] std::set<Entry> listImpl(std::string_view path) const override;

  private:
	std::unordered_map<std::string, std::string> mMemoryMap;

	[[nodiscard]] std::stringstream openStream(std::string_view path) const;
};

} // namespace spatial
