#pragma once

#include <GLFW/glfw3.h>
#include <string_view>

namespace spatial {

	class Window {
	private:
		GLFWwindow* m_windowHandle;
		Window(int width, int height, std::string_view title);

	public:
		~Window();
		Window(Window&& other) noexcept;
		Window& operator=(Window&& other) noexcept;

		Window(const Window& w) = delete;
		Window& operator=(const Window& w) = delete;

		bool isClosed();
		std::pair<int, int> getFrameBufferSize();
		void swapBuffers();

		friend class WindowManager;
	};

	class WindowManager {
	public:
		WindowManager();
		~WindowManager();

		void poolEvents();

		Window createWindow(int width, int height, std::string_view title);
	};

}