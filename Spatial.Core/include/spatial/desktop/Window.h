#pragma once

#include <SDL.h>
#include <math/vec2.h>
#include <spatial/common/EventQueue.h>
#include <spatial/common/Key.h>
#include <spatial/common/Signal.h>
#include <string_view>
#include <type_traits>

namespace spatial
{

Key mapKeyFromScancode(const SDL_Scancode scanCode) noexcept;

Key mapKeyFromMouseButton(int mouseButton) noexcept;

class DesktopPlatformContext;

class Window
{
  private:
	SDL_Window* mWindowHandle;

	Window(int width, int height, std::string_view title);

	friend class DesktopPlatformContext;

  public:
	~Window();

	void* getNativeHandle() const;
	SDL_Window* getHandle() const
	{
		return mWindowHandle;
	}

	std::pair<int, int> getFrameBufferSize() const;
	std::pair<int, int> getWindowSize() const;

	bool hasFocus() const;

	Window(Window&& other) noexcept;
	Window(const Window& w) = delete;

	Window& operator=(Window&& other) noexcept;
	Window& operator=(const Window& w) = delete;
	void warpMouse(filament::math::float2 position);
};

class DesktopPlatformContext
{
  private:
	static bool sValid;
	static EventQueue sEventQueue;

  public:
	DesktopPlatformContext();
	~DesktopPlatformContext();

	void onStartFrame(float);

	[[nodiscard]] Window createWindow(std::uint16_t width, std::uint16_t height, std::string_view title) const noexcept;

	DesktopPlatformContext(const DesktopPlatformContext& c) = delete;
	DesktopPlatformContext& operator=(const DesktopPlatformContext& w) = delete;

	DesktopPlatformContext(DesktopPlatformContext&& c) noexcept = delete;
	DesktopPlatformContext& operator=(DesktopPlatformContext&& other) noexcept = delete;

	auto& getEventQueue()
	{
		return sEventQueue;
	}
};

} // namespace spatial