#include <spatial\Window.h>
#include <utility>
#include <cassert>

namespace spatial {

Window::Window(int width, int height, std::string_view title)
	: m_windowHandle(glfwCreateWindow(width, height, title.data(), nullptr, nullptr)),
		m_drawable(nullptr)
{
	glfwMakeContextCurrent(m_windowHandle);
	glfwSwapInterval(1);
}

Window::~Window()
{
	glfwDestroyWindow(m_windowHandle);
}

void Window::bindDrawable(WindowDrawable* drawable) {
	m_drawable = drawable;
}

void Window::draw() {
	assert(m_drawable != nullptr, "Window without drawable bind");
	m_drawable->draw(*this);
}

bool Window::isClosed() const {
	return glfwWindowShouldClose(m_windowHandle);
}

void Window::swapBuffers() {
	glfwSwapBuffers(m_windowHandle);
}

void Window::makeCurrentContext() {
	glfwMakeContextCurrent(m_windowHandle);
}

std::pair<int, int> Window::getFrameBufferSize() const
{
	int dsw, dsh;
	glfwGetFramebufferSize(m_windowHandle, &dsw, &dsh);

	return {dsw, dsh};
}

}