#include <cassert>
#include <spatial/desktop/Window.h>
#include <utility>

#if defined(SPATIAL_PLATFORM_OSX)
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
	if (mWindowHandle != nullptr)
		glfwDestroyWindow(mWindowHandle);

	mWindowHandle = std::exchange(other.mWindowHandle, nullptr);

	return *this;
}

Window::~Window()
{
	if (mWindowHandle != nullptr)
		glfwDestroyWindow(mWindowHandle);
}

math::uvec2 Window::getFrameBufferSize() const
{
	int dsw;
	int dsh;
	glfwGetFramebufferSize(mWindowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

math::uvec2 Window::getSize() const
{
	int w;
	int h;
	glfwGetWindowSize(mWindowHandle, &w, &h);

	return {w, h};
}

math::dvec2 Window::getMousePosition() const
{
	double xPos;
	double yPos;
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
	return (void*)glfwGetCocoaWindow(mWindowHandle);
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

void Window::setClipboardText(const std::string& text)
{
	glfwSetClipboardString(mWindowHandle, text.c_str());
}

std::optional<std::string> Window::getClipboardText() const
{
	const auto* text = glfwGetClipboardString(mWindowHandle);
	if (text == nullptr)
	{
		return std::nullopt;
	}

	return std::make_optional(std::string(text));
}

} // namespace spatial::desktop