#pragma once

#include <entt/core/hashed_string.hpp>
#include <string_view>
#include <vector>

namespace spatial
{

using HashedString = entt::hashed_string;
using HashedWString = entt::hashed_wstring;

using namespace entt::literals;

std::vector<std::string_view> split(std::string_view value, char separator = ' ');

HashedString::hash_type hashOf(std::string_view value);
HashedWString::hash_type hashOf(std::wstring_view value);

} // namespace spatial
