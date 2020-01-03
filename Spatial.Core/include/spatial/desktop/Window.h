#pragma once

#include <string_view>
#include <type_traits>
#include <SDL.h>
#include <spatial/common/Key.h>

namespace spatial::desktop
{

common::Key mapKeyFromScancode(SDL_Scancode scanCode);

common::Key mapKeyFromMouseButton(int mouseButton);

class WindowContext;

class Window
{
private:
	SDL_Window *m_windowHandle;

	Window(int width, int height, std::string_view title);

	friend class WindowContext;

public:
	~Window();

	void onStartRender();
	void onEndRender();

	void* getNativeHandle();

	std::pair<uint32_t, uint32_t> getFrameBufferSize() const;
	std::pair<int, int> getWindowSize() const;

	Window(Window &&other) noexcept;
	Window(const Window &w) = delete;

	Window &operator=(Window &&other) noexcept;
	Window &operator=(const Window &w) = delete;
};

class WindowContext
{
public:
	WindowContext();
	~WindowContext();

	void pollEvents();

	Window createWindow(int width, int height, std::string_view title);

	WindowContext(const WindowContext &c) = delete;
	WindowContext(const WindowContext &&c) = delete;

	WindowContext &operator=(WindowContext &&other) = delete;
	WindowContext &operator=(const WindowContext &w) = delete;
};

} // namespace spatial::desktop