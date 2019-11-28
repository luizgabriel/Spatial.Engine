#include <spatial/hello.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>

static void error_callback(int error, const char* description)
{
	std::cerr << fmt::format("Error {0}: {1}\n", error, description);
}

int spatial::test()
{
	// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;
	GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL2 example", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();


		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0x00, 0x66, 0x99, 0x00);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void spatial::hello()
{
	std::cout << "Hello, world!" << std::endl;
}
