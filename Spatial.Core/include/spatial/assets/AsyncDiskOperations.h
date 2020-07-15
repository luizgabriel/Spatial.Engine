#pragma once

#include <future>
#include <optional>
#include <string_view>
#include <vector>

namespace spatial
{

template <typename DiskLoader, typename BufferType>
std::future<bool> asyncRead(const DiskLoader& disk, const std::string_view filePath, BufferType buffer) noexcept
{
	auto read = [](const auto& disk, const auto filePath, auto buffer) {
	  return disk(filePath, buffer);
	};
	return std::async(std::launch::async, std::move(read), disk, filePath, buffer);
}

template <typename Container = std::vector<char>, typename DiskLoader>
std::future<std::optional<Container>> asyncReadAll(const DiskLoader& disk, const std::string_view filePath) noexcept
{
	auto read = [](const auto& disk, const auto filePath) -> std::optional<Container> {
	  Container out{};
	  if (disk(filePath, std::back_inserter(out)))
		  return out;
	  else
		  return std::nullopt;
	};

	return std::async(std::launch::async, std::move(read), disk, filePath);
}

} // namespace spatial