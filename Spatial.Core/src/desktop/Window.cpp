#include <spatial/desktop/Window.h>
#include <spatial/desktop/native/WindowHelper.h>

#include <cassert>
#include <utility>

using namespace filament::math;

namespace spatial
{

Window::Window(int width, int height, std::string_view title)
	: mWindowHandle{SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
									 SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE)}
{
	assert(mWindowHandle != nullptr);
	// SDL_SetWindowBordered(mWindowHandle, SDL_FALSE);
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

void Window::warpMouse(filament::math::float2 position)
{
	SDL_WarpMouseInWindow(mWindowHandle, position.x, position.y);
}

} // namespace spatial