#include <spatial/hello.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>
#include <spatial/WindowManager.h>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

namespace spatial {

	struct MyDrawable: public WindowDrawable
	{
		void draw(const Window& window) override {
			auto [dw, dh] = window.getFrameBufferSize();

			glViewport(0, 0, dw, dh);
			glClearColor(0x00, 0x33, 0x66, 0x00);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	};

	int test()
	{
		WindowManager manager;
		MyDrawable drawable;
	
		auto& w1 = manager.createWindow(1280, 720, "Spatial Engine");
		w1.bindDrawable(&drawable);

		auto& w2 = manager.createWindow(800, 600, "Spatial Engine | Aux Window");
		w2.bindDrawable(&drawable);

		manager.mainLoop();

		return 0;
	}

	void hello()
	{
		std::cout << "Hello, world!" << std::endl;
	}

}