#pragma once

#include <string_view>
#include <spatial/common/Math.h>

namespace spatial::ui
{

class Window
{
  public:
	explicit Window(const std::string_view name);
	~Window();

	[[nodiscard]] math::float2 getSize() const;
	void setSize(const math::float2& size);

	[[nodiscard]] math::float2 getPosition() const;
	void setPosition(const math::float2& size);

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] bool isFocused() const;

  private:
	bool mIsOpen;
};

} // namespace spatial::ui
