#pragma once

#include <spatial/common/Math.h>

namespace spatial::ui
{

class WindowPaddingStyle
{
  public:
	WindowPaddingStyle();
	WindowPaddingStyle(const math::vec2& padding);
	~WindowPaddingStyle();
};

} // namespace spatial::ui