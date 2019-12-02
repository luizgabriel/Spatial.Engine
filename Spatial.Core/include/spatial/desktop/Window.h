#pragma once

#include <GLFW/glfw3.h>
#include <string_view>

namespace spatial
{
	class WindowContext;

	class Window {
	private:
		GLFWwindow* m_windowHandle;
		Window(int width, int height, std::string_view title);

		friend class WindowContext;
	public:
		~Window();

		void draw();

		bool isClosed() const;

		void swapBuffers();

		void makeCurrentContext();

		std::pair<int, int> getFrameBufferSize() const;

		Window(Window&& other);
		Window(const Window& w) = delete;

		Window& operator=(Window&& other);
		Window& operator=(const Window& w) = delete;
	};

	class WindowContext {
		public:
			WindowContext();
			~WindowContext();
			
			void pollEvents();

			Window createWindow(int width, int height, std::string_view title);
	};

}