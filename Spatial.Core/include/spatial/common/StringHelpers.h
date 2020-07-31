#pragma once

#include <span>
#include <string_view>
#include <vector>

namespace spatial
{

std::vector<std::string_view> split(std::string_view value, char separator = ' ');

} // namespace spatial
