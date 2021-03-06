#include <cassert>
#include <spatial/desktop/Window.h>
#include <utility>

#if defined(SPATIAL_PLATFORM_OSX)
#include <spatial/desktop/native/CocoaHelper.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(SPATIAL_PLATFORM_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SPATIAL_PLATFORM_UNIX)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

using namespace filament::math;

namespace spatial::desktop
{

Window::Window(GLFWwindow* windowHandle) : mWindowHandle{windowHandle}
{
	assert(mWindowHandle != nullptr);
}

Window::Window(Window&& other) noexcept : mWindowHandle(std::exchange(other.mWindowHandle, nullptr))
{
}

Window& Window::operator=(Window&& other) noexcept
{
	if (mWindowHandle)
		glfwDestroyWindow(mWindowHandle);

	mWindowHandle = std::exchange(other.mWindowHandle, nullptr);

	return *this;
}

Window::~Window()
{
	if (mWindowHandle)
		glfwDestroyWindow(mWindowHandle);
}

math::int2 Window::getFrameBufferSize() const
{
	int dsw, dsh;
	glfwGetFramebufferSize(mWindowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

math::int2 Window::getSize() const
{
	int w, h;
	glfwGetWindowSize(mWindowHandle, &w, &h);

	return {w, h};
}

math::double2 Window::getMousePosition() const
{
	double xPos, yPos;
	glfwGetCursorPos(mWindowHandle, &xPos, &yPos);

	return {xPos, yPos};
}

bool Window::hasFocus() const
{
	return glfwGetWindowAttrib(mWindowHandle, GLFW_FOCUSED) == GLFW_TRUE;
}

void* Window::getNativeHandle() const
{
#if defined(SPATIAL_PLATFORM_OSX)
	return ::cocoaGetContentViewFromWindow(glfwGetCocoaWindow(mWindowHandle));
#elif defined(SPATIAL_PLATFORM_WINDOWS)
	return (void*)glfwGetWin32Window(mWindowHandle);
#elif defined(SPATIAL_PLATFORM_UNIX)
	return (void*)glfwGetX11Window(mWindowHandle);
#else
	return nullptr;
#endif
}

void Window::warpMouse(const math::float2& position) const
{
	glfwSetCursorPos(mWindowHandle, position.x, position.y);
}

} // namespace spatial