#pragma once

#include <string_view>
#include <vector>
#include <optional>

namespace spatial::assets
{

using Resource = std::optional<std::vector<char>>;
using ResourcesLoader = std::function<Resource(std::string_view)>;

} // namespace spatial::assets