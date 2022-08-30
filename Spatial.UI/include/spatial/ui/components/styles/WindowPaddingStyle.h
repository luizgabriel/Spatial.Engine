#pragma once

#include <spatial/common/Math.h>

namespace spatial::ui
{

struct WindowPaddingStyle
{
	WindowPaddingStyle(math::vec2 padding = math::vec2{.0f});
	~WindowPaddingStyle();
};

} // namespace spatial::ui