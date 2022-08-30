#pragma once

#include <GLFW/glfw3.h>
#include <spatial/common/EventQueue.h>
#include <spatial/common/Key.h>
#include <spatial/common/Math.h>
#include <spatial/common/Signal.h>
#include <spatial/resources/FileSystem.h>
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

	void setIcon(const uint8_t* pixelsData, uint32_t size);
	void setIcon(FileSystem& fileSystem, std::string_view resourcePath);

	void* getNativeHandle() const;
	GLFWwindow* getHandle() const
	{
		return mWindowHandle;
	}

	math::uvec2 getFrameBufferSize() const;
	math::uvec2 getSize() const;
	math::dvec2 getMousePosition() const;

	bool hasFocus() const;

	Window(Window&& other) noexcept;
	Window(const Window& w) = delete;

	Window& operator=(Window&& other) noexcept;
	Window& operator=(const Window& w) = delete;

	void warpMouse(const math::vec2& position) const;
};

class PlatformContext
{
  public:
	PlatformContext();
	~PlatformContext();

	void onStartFrame(float);

	[[nodiscard]] Window createWindow(math::uvec2 dimensions, std::string_view title) const noexcept;

	PlatformContext(const PlatformContext& c) = delete;
	PlatformContext& operator=(const PlatformContext& w) = delete;

	PlatformContext(PlatformContext&& c) noexcept = delete;
	PlatformContext& operator=(PlatformContext&& other) noexcept = delete;

	auto& getEventQueue()
	{
		return sEventQueue;
	}

	math::uvec2 getMonitorSize();

  private:
	static bool sValid;
	static EventQueue sEventQueue;

	static void setupCallbacks(const Window& window);
};

} // namespace spatial::desktop