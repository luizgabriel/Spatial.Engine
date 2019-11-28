#pragma once

#include <spatial/Window.h>
#include <list>

namespace spatial
{
	class WindowManager {
	private:
		std::list<Window> m_windows;
	public:
		WindowManager();
		~WindowManager();

		Window& createWindow(int width, int height, std::string_view title);

		void mainLoop();

		void poolEvents();

		inline const Window& getMainWindow() const {
			return m_windows.front();
		}
	};

}