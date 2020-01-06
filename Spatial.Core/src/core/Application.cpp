#include <fmt/format.h>
#include <iostream>
#include <spatial/core/Application.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/EBus.h>

namespace spatial::core
{

Application::Application()
    : m_windowContext{},
      m_input{},
      m_rendering{m_windowContext.createWindow(1280, 720, "Spatial Engine")},
      m_simulation{}
{
    common::EBus::connect<desktop::WindowClosedEvent>(this);
}

Application::~Application()
{
    common::EBus::disconnect<desktop::WindowClosedEvent>(this);
}

void Application::onEvent(const desktop::WindowClosedEvent& event)
{
    stop();
}

void Application::stop()
{
    m_running = false;
}

int Application::run()
{
    physics::delta_t delta;
    m_running = true;

    m_startEvent.trigger();

    while (m_running)
    {
        delta = m_simulation.getDeltaTime();

        m_input.resetInputState();
        m_windowContext.pollEvents();
        common::EBus::update();

        m_updateEvent.trigger(delta.count());

        m_rendering.onRender();
        m_simulation.process();
    }

    m_finishEvent.trigger();

    return 0;
}


} // namespace spatial