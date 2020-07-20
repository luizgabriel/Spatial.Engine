#pragma once

#include <future>
#include <optional>
#include <string_view>
#include <vector>

namespace spatial::assets
{

std::optional<std::vector<char>> read(const auto& disk, const std::string_view filePath) noexcept
{
	std::vector<char> out{};
	if (disk(filePath, std::back_inserter(out)))
		return std::move(out);
	else
		return std::nullopt;
}

std::future<std::optional<std::vector<char>>> asyncRead(const auto& disk, const std::string_view filePath) noexcept
{
	auto read = [](const auto& disk, const auto filePath) -> std::optional<std::vector<char>> {
		return readAll(disk, filePath);
	};

	return std::async(std::launch::async, std::move(read), disk, filePath);
}

} // namespace spatial::assets