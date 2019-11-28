#include <spatial\Window.h>
#include <fmt/format.h>
#include <utility>
#include <iostream>

namespace spatial {

Window::Window(int width, int height, std::string_view title)
	: m_windowHandle(glfwCreateWindow(width, height, title.data(), nullptr, nullptr))
{
	glfwMakeContextCurrent(m_windowHandle);
	glfwSwapInterval(1);
}

Window::~Window()
{
	glfwDestroyWindow(m_windowHandle);
}

Window::Window(Window&& other) noexcept
	: m_windowHandle(std::exchange(other.m_windowHandle, nullptr))
{

}

Window& Window::operator=(Window&& other) noexcept
{
	glfwDestroyWindow(m_windowHandle);

	m_windowHandle = std::exchange(other.m_windowHandle, nullptr);
	return *this;
}

bool Window::isClosed()
{
	return glfwWindowShouldClose(m_windowHandle);
}

std::pair<int, int> Window::getFrameBufferSize()
{
	int dsw, dsh;
	glfwGetFramebufferSize(m_windowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_windowHandle);
}


WindowManager::WindowManager()
{
	glfwSetErrorCallback([] (int error, const char* description) {
		std::cerr << fmt::format("Error {0}: {1}\n", error, description);
	});

	if (!glfwInit())
		throw std::exception("Could not initialize GLFW");
}

void WindowManager::poolEvents()
{
	glfwPollEvents();
}

WindowManager::~WindowManager()
{
	glfwTerminate();
}

Window WindowManager::createWindow(int width, int height, std::string_view title)
{
	return Window(width, height, title);
}

}