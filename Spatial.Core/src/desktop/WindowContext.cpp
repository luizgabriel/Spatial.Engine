#include <spatial/desktop/Window.h>

namespace spatial::desktop
{

WindowContext::WindowContext()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Could not initialize GLFW");
    }
}

WindowContext::~WindowContext()
{
    glfwTerminate();
}

void WindowContext::pollEvents()
{
    glfwPollEvents();
    m_eventQueue.update();
}

Window WindowContext::createWindow(int width, int height, std::string_view title)
{
    auto window = Window{width, height, title};
    window.setEventQueue(&m_eventQueue);

    return window;
}

} // namespace spatial::desktop