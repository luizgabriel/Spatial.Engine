#pragma once

#include <string_view>
#include <vector>

namespace spatial::assets
{

using ResourceLoaderIterator = std::back_insert_iterator<std::vector<char>>;
using ResourcesLoader = std::function<bool(std::string_view, ResourceLoaderIterator)>;

} // namespace spatial::assets