#include <fmt/format.h>
#include <iostream>
#include <spatial/Application.h>
#include <spatial/desktop/PlatformEvent.h>

namespace spatial
{

void printMousePosition(const desktop::MouseMovedEvent &event)
{
    std::cout << fmt::format("MOUSE: ({0}, {1})\n", event.x, event.y);
}

void printWindowResized(const desktop::WindowResizedEvent &event)
{
    std::cout << fmt::format("RESIZE: ({0}, {1})\n", event.width, event.height);
}

void Application::onWindowClose(const desktop::WindowClosedEvent& event)
{
    stop();
}

void Application::stop()
{
    m_running = false;
}

int Application::run()
{
    m_running = true;

    m_windowContext.connect<desktop::MouseMovedEvent, &printMousePosition>();
    m_windowContext.connect<desktop::WindowResizedEvent, &printWindowResized>();
    m_windowContext.connect<desktop::WindowClosedEvent, &Application::onWindowClosed>();
    m_windowContext.connect<desktop::WindowResizedEvent, &RenderingSubsystem::onWindowResized>(m_rendering);

    while (!m_running)
    {
        m_windowContext.pollEvents();
        m_rendering.onRender();
    }

    return 0;
}

Application::Application()
    : m_windowContext{},
      m_mainWindow{m_windowContext.createWindow(1280, 720, "Spatial Engine")},
      m_input{m_windowContext},
      m_rendering{m_mainWindow}
{
}

} // namespace spatial