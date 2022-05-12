#pragma once

#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <future>

namespace spatial
{

class FileSystem;

using SharedFileSystem = std::shared_ptr<FileSystem>;

class FileSystem
{
  public:
	static constexpr auto SEPARATOR = '/';

	struct ResolveResult
	{
		std::string path;
		FileSystem& fileSystem;
	};

	enum class FileType
	{
		File,
		Directory
	};

	struct Entry
	{
		std::string path;
		FileType type;

		Entry(std::string path, FileType type) : path{std::move(path)}, type{type}
		{
		}

		[[nodiscard]] bool isDirectory() const noexcept
		{
			return type == FileType::Directory;
		}

		[[nodiscard]] bool isFile() const noexcept
		{
			return type == FileType::File;
		}

		bool operator<(const Entry& rhs) const
		{
			if (path < rhs.path)
				return true;
			if (rhs.path < path)
				return false;
			return type < rhs.type;
		}

		bool operator>(const Entry& rhs) const
		{
			return rhs < *this;
		}

		bool operator<=(const Entry& rhs) const
		{
			return !(rhs < *this);
		}

		bool operator>=(const Entry& rhs) const
		{
			return !(*this < rhs);
		}
	};

	virtual ~FileSystem() = default;

	template <typename FileSystemImpl, typename... Args>
	std::shared_ptr<FileSystemImpl> mount(std::string_view key, Args&&... args)
	{
		auto fileSystem = std::make_shared<FileSystemImpl>(std::forward<Args>(args)...);
		mount(key, fileSystem);
		return fileSystem;
	}

	void mount(std::string_view key, const SharedFileSystem& child);

	ResolveResult resolve(std::string_view path) noexcept;

	std::unique_ptr<std::istream> openReadStream(std::string_view path) noexcept;

	std::unique_ptr<std::ostream> openWriteStream(std::string_view path) noexcept;

	std::set<Entry> list(std::string_view path) noexcept;

	std::string readString(std::string_view path) noexcept;

	std::vector<uint8_t> readBinary(std::string_view path) noexcept;

	std::future<std::vector<uint8_t>> readBinaryAsync(std::string_view path) noexcept;

	bool write(std::string_view path, std::string&& data) noexcept;

  private:
	std::map<std::string, SharedFileSystem> mNodes;

  protected:
	[[nodiscard]] virtual std::unique_ptr<std::istream> openReadStreamImpl(std::string_view path) noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<std::ostream> openWriteStreamImpl(std::string_view path) noexcept = 0;

	[[nodiscard]] virtual std::set<Entry> listImpl(std::string_view path) const = 0;
};

class AggregateFileSystem : public FileSystem
{
  public:
	AggregateFileSystem() = default;

  protected:
	[[nodiscard]] std::unique_ptr<std::istream> openReadStreamImpl(std::string_view path) noexcept override;

	[[nodiscard]] std::unique_ptr<std::ostream> openWriteStreamImpl(std::string_view path) noexcept override;

	[[nodiscard]] std::set<Entry> listImpl(std::string_view path) const override;

  private:
	[[nodiscard]] static std::unique_ptr<std::iostream> makeFailedStream() noexcept;
};

} // namespace spatial
