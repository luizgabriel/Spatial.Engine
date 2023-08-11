#pragma once

#include <string>

namespace spatial::ui
{

struct ClipboardGetEvent
{
	std::string value;
};

struct ClipboardSetEvent
{
	std::string value;
};

} // namespace spatial::ui