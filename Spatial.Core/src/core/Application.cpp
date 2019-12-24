#include <fmt/format.h>
#include <iostream>
#include <spatial/core/Application.h>
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

void Application::onWindowClosed(const desktop::WindowClosedEvent& event)
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
    m_windowContext.connect<desktop::WindowClosedEvent, &Application::onWindowClosed>(this);
    m_windowContext.connect<desktop::WindowResizedEvent, &render::RenderingSubsystem::onWindowResized>(&m_rendering);

    onStartEvent();

    physics::delta_t delta;

    while (m_running)
    {
        delta = m_simulation.getDeltaTime();

        m_windowContext.pollEvents();
        m_rendering.onRender();

        onUpdateEvent(delta.count());

        m_simulation.process();
    }

    onFinishEvent();

    return 0;
}

Application::Application()
    : m_windowContext{},
      m_input{m_windowContext},
      m_rendering{m_windowContext.createWindow(1280, 720, "Spatial Engine")},
      m_simulation{}
{
}

} // namespace spatial