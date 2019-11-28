#include <spatial/WindowManager.h>
#include <fmt/format.h>
#include <iostream>

namespace spatial
{
	WindowManager::WindowManager()
		: m_windows{}
	{
		glfwSetErrorCallback([](int error, const char* description) {
			std::cerr << fmt::format("Error {0}: {1}\n", error, description);
		});

		if (!glfwInit())
			throw std::exception("Could not initialize GLFW");
	}

	WindowManager::~WindowManager()
	{
		glfwTerminate();
	}

	void WindowManager::poolEvents()
	{
		glfwPollEvents();
	}

	void WindowManager::mainLoop()
	{
		auto& mainWindow = getMainWindow();

		while (!mainWindow.isClosed()) {
			poolEvents();

			for (auto& window : m_windows) {
				window.makeCurrentContext();
				window.draw();
				window.swapBuffers();
			}
		}
	}

	Window& WindowManager::createWindow(int width, int height, std::string_view title)
	{
		m_windows.emplace_back(width, height, title);
		return m_windows.back();
	}

}