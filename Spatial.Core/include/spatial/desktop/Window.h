#pragma once

#include <string_view>
#include <type_traits>
#include <SDL.h>
#include <spatial/common/Key.h>
#include <spatial/common/EventQueue.h>

namespace spatial::desktop
{

common::Key mapKeyFromScancode(const SDL_Scancode scanCode) noexcept;

common::Key mapKeyFromMouseButton(int mouseButton) noexcept;

class WindowContext;

class Window
{
private:
	SDL_Window* m_windowHandle;

	Window(int width, int height, std::string_view title);

	friend class WindowContext;

public:
	~Window();

	void* getNativeHandle() const;
	std::pair<uint32_t, uint32_t> getFrameBufferSize() const;
	std::pair<int, int> getWindowSize() const;
	bool hasFocus() const;

	Window(Window&& other) noexcept;
	Window(const Window& w) = delete;

	Window& operator=(Window&& other) noexcept;
	Window& operator=(const Window& w) = delete;
};

class WindowContext
{
public:
	WindowContext();
	~WindowContext();

	void pollEvents(common::EventQueue& queue);

	[[nodiscard]] Window createWindow(int width, int height, std::string_view title) const noexcept;

	WindowContext(const WindowContext& c) = delete;
	WindowContext(const WindowContext&& c) = delete;

	WindowContext& operator=(WindowContext&& other) = delete;
	WindowContext& operator=(const WindowContext& w) = delete;
};

} // namespace spatial::desktop