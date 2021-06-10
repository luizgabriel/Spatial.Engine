#pragma once

#include <span>
#include <string_view>
#include <vector>
#include <entt/core/hashed_string.hpp>

namespace spatial
{

using HashedString = entt::hashed_string;
using HashedWString = entt::hashed_wstring;

using namespace entt::literals;

std::vector<std::string_view> split(std::string_view value, char separator = ' ');

} // namespace spatial
