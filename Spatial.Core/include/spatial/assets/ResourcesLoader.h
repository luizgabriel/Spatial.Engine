#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace spatial::assets
{

using ResourceData = std::optional<std::vector<char>>;

struct ResourcesLoader
{
	virtual ~ResourcesLoader() = default;
	virtual ResourceData load(const std::string_view resourceName) const = 0;
};

} // namespace spatial::assets