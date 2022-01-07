#pragma once

#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

enum class WindowFlags {
	None = 0,
	NoTitleBar = 1 << 0,
	NoScrollbar = 1 << 3,
};

WindowFlags operator|(WindowFlags l, WindowFlags r);

class Window
{
  public:
	explicit Window(std::string_view name, WindowFlags flags = WindowFlags::None);
	~Window();

	[[nodiscard]] math::float2 getSize() const;
	void setSize(const math::float2& size);

	[[nodiscard]] math::float2 getPosition() const;
	void setPosition(const math::float2& size);

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] bool isFocused() const;

	template <typename Function>
	static void show(const std::string_view name, Function func)
	{
		auto window = Window{name};
		if (window.isOpen())
			func();
	}

  protected:
	bool mIsOpen;
};

} // namespace spatial::ui
