#pragma once

#include <future>
#include <map>
#include <memory>
#include <set>
#include <string_view>

namespace spatial
{

class FileSystem;

using SharedFileSystem = std::shared_ptr<FileSystem>;

class FileSystem
{
  public:
	static constexpr auto SEPARATOR = '/';

	enum class EntryType
	{
		File,
		Directory
	};

	struct Entry
	{
		std::string path;
		EntryType type;

		Entry(std::string path, EntryType type) : path{std::move(path)}, type{type}
		{
		}

		[[nodiscard]] bool isDirectory() const noexcept
		{
			return type == EntryType::Directory;
		}

		[[nodiscard]] bool isFile() const noexcept
		{
			return type == EntryType::File;
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

	[[nodiscard]] std::string readString(std::string_view path) noexcept;

	[[nodiscard]] std::vector<uint8_t> readBinary(std::string_view path) noexcept;

	[[nodiscard]] std::future<std::vector<uint8_t>> readBinaryAsync(std::string_view path) noexcept;

	bool write(std::string_view path, std::string&& data) noexcept;

	[[nodiscard]] virtual std::unique_ptr<std::istream> openReadStream(std::string_view path) noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<std::ostream> openWriteStream(std::string_view path) noexcept = 0;

	[[nodiscard]] virtual std::set<Entry> list(std::string_view path) const = 0;
};

std::pair<std::string_view, std::string_view> splitPath(std::string_view path) noexcept;

} // namespace spatial
