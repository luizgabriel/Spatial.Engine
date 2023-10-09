#pragma once

#include <GLFW/glfw3.h>
#include <spatial/common/EventQueue.h>
#include <spatial/common/Key.h>
#include <spatial/common/Math.h>
#include <spatial/common/Signal.h>
#include <spatial/resources/FileSystem.h>
#include <string_view>
#include <optional>

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
	EventQueue mEventQueue;

	explicit Window(GLFWwindow* windowHandle);

	friend class PlatformContext;

	void setupCallbacks();

  public:
	~Window();

	void setIcon(const uint8_t* pixelsData, uint32_t size);
	void setIcon(FileSystem& fileSystem, std::string_view resourcePath);

	[[nodiscard]] void* getNativeHandle() const;
	[[nodiscard]] GLFWwindow* getHandle() const
	{
		return mWindowHandle;
	}

	void setClipboardText(const std::string& text);

	[[nodiscard]] std::optional<std::string> getClipboardText() const;

	math::uvec2 getFrameBufferSize() const;
	math::uvec2 getSize() const;
	math::dvec2 getMousePosition() const;

	bool hasFocus() const;

	Window(Window&& other) noexcept;
	Window(const Window& w) = delete;

	Window& operator=(Window&& other) noexcept;
	Window& operator=(const Window& w) = delete;

	void warpMouse(const math::vec2& position) const;

	void onStartFrame(float);

	auto& getEventQueue()
	{
		return mEventQueue;
	}
};

class PlatformContext
{
  public:
	PlatformContext();
	~PlatformContext();

	void onStartFrame(float);

	[[nodiscard]] Window createWindow(math::uvec2 dimensions, std::string_view title) const;

	PlatformContext(const PlatformContext& c) = delete;
	PlatformContext& operator=(const PlatformContext& w) = delete;

	PlatformContext(PlatformContext&& c) noexcept = delete;
	PlatformContext& operator=(PlatformContext&& other) noexcept = delete;

	math::uvec2 getMonitorSize();
};

} // namespace spatial::desktop