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

std::string nameOf(KeyAction action);

} // namespace spatial::common
