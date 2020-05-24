#pragma once

#include <string_view>
#include <vector>

namespace spatial
{

std::vector<std::string_view> split(std::string_view str, char separator);

}
