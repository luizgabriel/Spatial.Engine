#pragma once

#include <GLFW/glfw3.h>
#include <spatial/common/EventQueue.h>
#include <spatial/common/Key.h>
#include <spatial/common/Signal.h>
#include <spatial/common/Math.h>
#include <string_view>
#include <type_traits>

namespace spatial::desktop
{

Key mapKeyFromScancode(const int scanCode) noexcept;

Key mapKeyFromMouseButton(int mouseButton) noexcept;

KeyAction mapActionFromCode(int action) noexcept;

class PlatformContext;

class Window
{
  private:
	GLFWwindow* mWindowHandle;

	explicit Window(GLFWwindow* windowHandle);

	friend class PlatformContext;

  public:
	~Window();

	void* getNativeHandle() const;
	GLFWwindow* getHandle() const
	{
		return mWindowHandle;
	}

	math::int2 getFrameBufferSize() const;
	math::int2 getSize() const;
	math::double2 getMousePosition() const;

	bool hasFocus() const;

	Window(Window&& other) noexcept;
	Window(const Window& w) = delete;

	Window& operator=(Window&& other) noexcept;
	Window& operator=(const Window& w) = delete;

	void warpMouse(const math::float2& position) const;
};

class PlatformContext
{
  public:
	PlatformContext();
	~PlatformContext();

	void onStartFrame(float);

	[[nodiscard]] Window createWindow(std::uint16_t width, std::uint16_t height, std::string_view title) const noexcept;

	PlatformContext(const PlatformContext& c) = delete;
	PlatformContext& operator=(const PlatformContext& w) = delete;

	PlatformContext(PlatformContext&& c) noexcept = delete;
	PlatformContext& operator=(PlatformContext&& other) noexcept = delete;

	auto& getEventQueue()
	{
		return sEventQueue;
	}

  private:
	static bool sValid;
	static EventQueue sEventQueue;

	static void setupCallbacks(GLFWwindow* window);
};

} // namespace spatial