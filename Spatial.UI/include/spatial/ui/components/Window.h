#pragma once

#include <string_view>
#include <spatial/common/Math.h>

namespace spatial::ui
{

class Window
{
  public:
	Window(const std::string_view name);
	~Window();

	math::float2 getSize() const;
	void setSize(const math::float2& size);

	math::float2 getPosition() const;
	void setPosition(const math::float2& size);

	bool isFocused() const;
};

} // namespace spatial::ui
