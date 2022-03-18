#pragma once

#include <filesystem>
#include <spatial/resources/FileSystem.h>

namespace spatial
{

class PhysicalFileSystem : public FileSystem
{
  public:
	PhysicalFileSystem() = default;
	explicit PhysicalFileSystem(std::filesystem::path rootPath);

	void setRootPath(const std::filesystem::path& rootPath);

  protected:
	[[nodiscard]] std::unique_ptr<std::istream> openReadStreamImpl(std::string_view path) noexcept override;

	[[nodiscard]] std::unique_ptr<std::ostream> openWriteStreamImpl(std::string_view path) noexcept override;

	[[nodiscard]] std::set<Entry> listImpl(std::string_view path) const override;

  private:
	std::filesystem::path mRootPath;

	[[nodiscard]] std::filesystem::path makeFullPath(std::string_view path) const noexcept;
};

} // namespace spatial
