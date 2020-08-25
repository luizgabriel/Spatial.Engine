#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace spatial::assets
{

using Resource = std::optional<std::vector<char>>;
using ResourcesLoader = std::function<Resource(const std::string_view)>;

} // namespace spatial::assets