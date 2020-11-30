#include <spatial/desktop/Window.h>
#include <spatial/desktop/native/WindowHelper.h>

#include <cassert>
#include <utility>

using namespace filament::math;

namespace spatial
{

Window::Window(SDL_Window* windowHandle)
	: mWindowHandle{windowHandle}
{
	assert(mWindowHandle != nullptr);
}

Window::Window(Window&& other) noexcept : mWindowHandle(std::exchange(other.mWindowHandle, nullptr))
{
}

Window& Window::operator=(Window&& other) noexcept
{
	if (mWindowHandle)
		SDL_DestroyWindow(mWindowHandle);

	mWindowHandle = std::exchange(other.mWindowHandle, nullptr);

	return *this;
}

Window::~Window()
{
	if (mWindowHandle)
		SDL_DestroyWindow(mWindowHandle);
}

std::pair<int, int> Window::getFrameBufferSize() const
{
	int dsw, dsh;
	SDL_GL_GetDrawableSize(mWindowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

std::pair<int, int> Window::getWindowSize() const
{
	int w, h;
	SDL_GetWindowSize(mWindowHandle, &w, &h);

	return {w, h};
}

bool Window::hasFocus() const
{
	return (SDL_GetWindowFlags(mWindowHandle) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

void* Window::getNativeHandle() const
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	SDL_GetWindowWMInfo(mWindowHandle, &wmi);
	return ::getNativeWindow(wmi);
}

void Window::warpMouse(math::float2 position)
{
	auto [width, height] = getWindowSize();
	position = position * math::float2{width, height};

	SDL_WarpMouseInWindow(mWindowHandle, position.x, position.y);
}



} // namespace spatial