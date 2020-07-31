#pragma once

#include <future>
#include <optional>
#include <string_view>
#include <vector>

namespace spatial::assets
{

std::future<Resource> asyncRead(const auto& disk, const std::string_view filePath) noexcept
{
	return std::async(
		std::launch::async, [](const auto& disk, const auto filePath) { return disk(filePath); }, disk, filePath);
}

} // namespace spatial::assets