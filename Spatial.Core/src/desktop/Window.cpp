#include <spatial/desktop/Window.h>
#include <utility>
#include <cassert>

#if !defined(SPATIAL_PLATFORM_WINDOWS)
#include <unistd.h>
#else
#include <SDL_syswm.h>
#include <utils/unwindows.h>
#endif

using namespace filament::math;

namespace spatial
{

Window::Window(int width, int height, std::string_view title)
	: m_windowHandle{SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
									  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE)}
{
	assert(m_windowHandle != nullptr);
	// SDL_SetWindowBordered(m_windowHandle, SDL_FALSE);
}

Window::Window(Window&& other) noexcept : m_windowHandle(std::exchange(other.m_windowHandle, nullptr))
{
}

Window& Window::operator=(Window&& other) noexcept
{
	if (m_windowHandle)
		SDL_DestroyWindow(m_windowHandle);

	m_windowHandle = std::exchange(other.m_windowHandle, nullptr);

	return *this;
}

Window::~Window()
{
	if (m_windowHandle)
		SDL_DestroyWindow(m_windowHandle);
}

std::pair<int, int> Window::getFrameBufferSize() const
{
	int dsw, dsh;
	SDL_GL_GetDrawableSize(m_windowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

std::pair<int, int> Window::getWindowSize() const
{
	int w, h;
	SDL_GetWindowSize(m_windowHandle, &w, &h);

	return {w, h};
}

bool Window::hasFocus() const
{
	return (SDL_GetWindowFlags(m_windowHandle) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

void* Window::getNativeHandle() const
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_windowHandle, &wmInfo);
	return wmInfo.info.win.window;
}

void Window::warpMouse(filament::math::float2 position)
{
	SDL_WarpMouseInWindow(m_windowHandle, position.x, position.y);
}

} // namespace spatial