#include <spatial/desktop/Window.h>
#include <utility>
#include <cassert>

namespace spatial {

Window::Window(int width, int height, std::string_view title)
	: m_windowHandle(glfwCreateWindow(width, height, title.data(), nullptr, nullptr))
{
	glfwMakeContextCurrent(m_windowHandle);
	glfwSwapInterval(1);
}

Window::Window(Window&& other) 
	: m_windowHandle(std::exchange(other.m_windowHandle, nullptr))
{
}

Window& Window::operator=(Window&& other) 
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

WindowContext::WindowContext()
{
	if (!glfwInit()) {
		throw std::runtime_error("Could not initialize GLFW");
	}
}

WindowContext::~WindowContext()
{
	glfwTerminate();
}

void WindowContext::pollEvents()
{
	glfwPollEvents();
}
			
Window WindowContext::createWindow(int width, int height, std::string_view title)
{
	return Window(width, height, title);
}

}