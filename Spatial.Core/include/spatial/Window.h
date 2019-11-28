#pragma once

#include <GLFW/glfw3.h>
#include <string_view>

namespace spatial
{
	class Window;

	class WindowDrawable {
	public:
		virtual void draw(const Window& window) = 0;
	};

	class Window {
	private:
		GLFWwindow* m_windowHandle;
		WindowDrawable* m_drawable;
		
	public:
		Window(int width, int height, std::string_view title);
		~Window();

		void bindDrawable(WindowDrawable* drawable);

		void draw();

		bool isClosed() const;

		void swapBuffers();

		void makeCurrentContext();

		std::pair<int, int> getFrameBufferSize() const;

		Window(Window&& other) = delete;
		Window(const Window& w) = delete;

		Window& operator=(Window&& other) = delete;
		Window& operator=(const Window& w) = delete;
	};

}