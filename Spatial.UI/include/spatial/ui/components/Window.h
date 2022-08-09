#pragma once

#include <spatial/common/Math.h>
#include <string_view>

namespace spatial::ui
{

enum class WindowFlags
{
	None = 0,
	NoTitleBar = 1 << 0,		// Disable title-bar
	NoResize = 1 << 1,			// Disable user resizing with the lower-right grip
	NoMove = 1 << 2,			// Disable user moving the window
	NoScrollbar = 1 << 3,		// Disable scrollbars (window can still scroll with mouse or programmatically)
	NoScrollWithMouse = 1 << 4, // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will
								// be forwarded to the parent unless NoScrollbar is also set.
	NoCollapse = 1 << 5, // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu
						 // Button (e.g. within a docking node).
	AlwaysAutoResize = 1 << 6, // Resize every window to its content every frame
	NoBackground = 1 << 7,	   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using
							   // SetNextWindowBgAlpha(0.0f).
	NoSavedSettings = 1 << 8,  // Never load/save settings in .ini file
	NoMouseInputs = 1 << 9,	   // Disable catching mouse, hovering test with pass through.
	MenuBar = 1 << 10,		   // Has a menu-bar
	HorizontalScrollbar = 1 << 11, // Allow horizontal scrollbar to appear (off by default). You may use
								   // SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify
								   // width. Read code in imgui_demo in the "Horizontal Scrolling" section.
	NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
	NoBringToFrontOnFocus = 1 << 13, // Disable bringing window to front when taking focus (e.g. clicking on it or

	NoNavInputs = 1 << 18, // No gamepad/keyboard navigation within the window
	NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)

	NoNav = NoNavInputs | NoNavFocus,
	NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
	NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,
};

WindowFlags operator|(WindowFlags l, WindowFlags r);

class Window
{
  public:
	explicit Window(std::string_view name, WindowFlags flags = WindowFlags::None);
	~Window();

	[[nodiscard]] math::vec2 getSize() const;
	void setSize(const math::vec2& size);

	[[nodiscard]] math::vec2 getPosition() const;
	void setPosition(const math::vec2& size);

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
