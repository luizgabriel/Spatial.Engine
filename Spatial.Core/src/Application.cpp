#include <fmt/format.h>
#include <iostream>
#include <spatial/Application.h>
#include <spatial/desktop/PlatformEvent.h>

namespace spatial
{

void printMousePosition(const desktop::MouseMoved &event)
{
    std::cout << fmt::format("MOUSE: ({0}, {1})\n", event.x, event.y);
}

void printWindowResized(const desktop::WindowResized &event)
{
    std::cout << fmt::format("RESIZE: ({0}, {1})\n", event.width, event.height);
}

int Application::run()
{
    m_windowContext.connect<desktop::MouseMoved, &printMousePosition>();
    m_windowContext.connect<desktop::WindowResized, &printWindowResized>();

    while (!m_mainWindow.isClosed())
    {
        m_windowContext.pollEvents();

        m_rendering.onStartFrame();

        //

        m_rendering.onEndFrame();
    }

    return 0;
}

Application::Application()
    : m_windowContext{},
      m_mainWindow{m_windowContext.createWindow(1280, 720, "Spatial Engine")},
      m_rendering{m_mainWindow}
{
}

} // namespace spatial