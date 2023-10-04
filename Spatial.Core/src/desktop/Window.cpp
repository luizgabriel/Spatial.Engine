#include <cassert>
#include <spatial/desktop/Window.h>
#include <utility>
#include <sstream>
#include <boost/locale.hpp>

#if defined(SPATIAL_PLATFORM_OSX)
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(SPATIAL_PLATFORM_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SPATIAL_PLATFORM_UNIX)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include "spatial/desktop/PlatformEvent.h"
#include <GLFW/glfw3native.h>
#include <array>
#include <stb_image.h>

namespace spatial::desktop
{

Window::Window(GLFWwindow* windowHandle) : mWindowHandle{windowHandle}, mEventQueue{}
{
	assert(mWindowHandle != nullptr);
	mEventQueue.enqueue<WindowResizedEvent>(this, getSize(), getFrameBufferSize());
	setupCallbacks();
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

void Window::onStartFrame(float)
{
	mEventQueue.template update<WindowResizedEvent>();
	mEventQueue.template update<MouseMovedEvent>();
	mEventQueue.update();
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

void Window::setupCallbacks()
{
	glfwSetWindowUserPointer(getHandle(), this);

	glfwSetWindowCloseCallback(getHandle(), [](auto* win) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		window->getEventQueue().enqueue<WindowClosedEvent>(window);
	});

	glfwSetCharCallback(getHandle(), [](auto* win, unsigned int codepoint) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		auto ss = std::wstringstream{};
		ss << static_cast<wchar_t>(codepoint);

		window->getEventQueue().enqueue<TextEvent>(window, boost::locale::conv::utf_to_utf<char>(ss.str()));
	});

	glfwSetKeyCallback(getHandle(), [](auto* win, int key, int scancode, int action, int mods) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		const auto spatialKey = mapKeyFromScancode(key);
		const auto spatialAction = mapActionFromCode(action);
		window->getEventQueue().enqueue<KeyEvent>(window, spatialKey, spatialAction);
	});

	glfwSetMouseButtonCallback(getHandle(), [](auto* win, int button, int action, int mods) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		const auto spatialKey = mapKeyFromMouseButton(button);
		const auto spatialAction = mapActionFromCode(action);
		window->getEventQueue().enqueue<KeyEvent>(window, spatialKey, spatialAction);
	});

	glfwSetScrollCallback(getHandle(), [](auto* win, double xOffset, double yOffset) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		window->getEventQueue().enqueue<MouseScrolledEvent>(window, math::dvec2{xOffset, yOffset});
	});

	glfwSetWindowSizeCallback(getHandle(), [](auto* win, int width, int height) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		int frameBufferWidth, frameBufferHeight;
		glfwGetFramebufferSize(win, &frameBufferWidth, &frameBufferHeight);
		window->getEventQueue().enqueue<WindowResizedEvent>(window, math::uvec2{width, height},
												math::uvec2{frameBufferWidth, frameBufferHeight});
	});

	glfwSetCursorPosCallback(getHandle(), [](auto* win, double xPos, double yPos) {
		auto* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win));
		assert(window != nullptr);

		window->getEventQueue().enqueue<MouseMovedEvent>(window, math::dvec2{xPos, yPos});
	});
}

} // namespace spatial::desktop