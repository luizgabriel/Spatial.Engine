#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <utility>
#include <cassert>

namespace spatial::desktop
{

Window::Window(int width, int height, std::string_view title)
	: m_windowHandle{glfwCreateWindow(width, height, title.data(), nullptr, nullptr)}
{
	glfwMakeContextCurrent(m_windowHandle);
	glfwSwapInterval(1);
}

Window::Window(Window &&other)
	: m_windowHandle(std::exchange(other.m_windowHandle, nullptr))
{
}

Window &Window::operator=(Window &&other)
{
	m_windowHandle = std::exchange(other.m_windowHandle, nullptr);

	return *this;
}

Window::~Window()
{
	if (m_windowHandle)
		glfwDestroyWindow(m_windowHandle);
}

bool Window::isClosed() const
{
	return glfwWindowShouldClose(m_windowHandle);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_windowHandle);
}

void Window::makeCurrentContext()
{
	glfwMakeContextCurrent(m_windowHandle);
}

std::pair<int, int> Window::getFrameBufferSize() const
{
	int dsw, dsh;
	glfwGetFramebufferSize(m_windowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

void Window::setEventQueue(common::EventQueue *queue)
{
	glfwSetWindowUserPointer(m_windowHandle, queue);

	glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *handle, int width, int height) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<WindowResized>(width, height);
	});

	glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *handle) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<WindowClosed>();
	});

	glfwSetKeyCallback(m_windowHandle, [](GLFWwindow *handle, int key, int scancode, int action, int mods) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<KeyTyped>(key, scancode);
	});

	glfwSetCharCallback(m_windowHandle, [](GLFWwindow *handle, unsigned int keycode) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<KeyTyped>(keycode, -1);
	});

	glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow *handle, int button, int action, int mods) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<MouseButton>(button, action);
	});

	glfwSetScrollCallback(m_windowHandle, [](GLFWwindow *handle, double xOffset, double yOffset) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<MouseScrolled>(xOffset, yOffset);
	});

	glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow *handle, double xPos, double yPos) {
		auto queue = reinterpret_cast<common::EventQueue *>(glfwGetWindowUserPointer(handle));
		queue->enqueue<MouseMoved>(xPos, yPos);
	});
}

} // namespace spatial::desktop