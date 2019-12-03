#pragma once

#include <string_view>

namespace spatial::common
{

enum class KeyAction
{
    None,
    Pressed,
    Released,
};

std::string_view nameOf(KeyAction action);

} // namespace spatial::common
