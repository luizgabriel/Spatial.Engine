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
#include <array>
#include <stb_image.h>

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

math::uvec2 Window::getFrameBufferSize() const
{
	int dsw, dsh;
	glfwGetFramebufferSize(mWindowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

math::uvec2 Window::getSize() const
{
	int w, h;
	glfwGetWindowSize(mWindowHandle, &w, &h);

	return {w, h};
}

math::dvec2 Window::getMousePosition() const
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

void Window::warpMouse(const math::vec2& position) const
{
	glfwSetCursorPos(mWindowHandle, position.x, position.y);
}

void Window::setIcon(const uint8_t* pixelsData, uint32_t size)
{
	auto icons = std::array<GLFWimage, 1>{};
	icons[0].pixels =
		stbi_load_from_memory(pixelsData, static_cast<int>(size), &icons[0].width, &icons[0].height, nullptr, 4);

	glfwSetWindowIcon(mWindowHandle, 1, &icons[0]);

	stbi_image_free(icons[0].pixels);
}

void Window::setIcon(FileSystem& fileSystem, std::string_view resourcePath)
{
	auto iconData = fileSystem.readBinary(resourcePath);
	assert(!iconData.empty());
	setIcon(iconData.data(), iconData.size());
}


} // namespace spatial::desktop