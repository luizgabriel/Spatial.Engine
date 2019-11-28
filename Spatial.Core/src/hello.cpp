#include <spatial/hello.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>
#include <spatial/Window.h>

int spatial::test()
{
	spatial::WindowManager manager;
	auto window = manager.createWindow(1280, 720, "Spatial Engine");

	// Main loop
	while (!window.isClosed())
	{
		manager.poolEvents();

		// Rendering
		auto [display_w, display_h] = window.getFrameBufferSize();

		glViewport(0, 0, display_w, display_h);
		glClearColor(0x00, 0x66, 0x99, 0x00);
		glClear(GL_COLOR_BUFFER_BIT);

		window.swapBuffers();
	}

	return 0;
}

void spatial::hello()
{
	std::cout << "Hello, world!" << std::endl;
}
