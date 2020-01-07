#include <fmt/format.h>
#include <iostream>
#include <spatial/core/Application.h>
#include <spatial/core/EBus.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/Logger.h>
#include <spatial/core/SystemSignals.h>

#include <chrono>

using namespace spatial::common;
using namespace spatial::desktop;

namespace spatial::core
{

Application::Application()
    : m_running{false},
      m_desiredDelta{1.0f / 120.0f},
      m_windowContext{},
      m_input{},
      m_rendering{m_windowContext.createWindow(1280, 720, "Spatial Engine")},
      m_simulation{}
{
    EBus::connect<WindowClosedEvent>(this);
}

Application::~Application()
{
    EBus::disconnect<WindowClosedEvent>(this);
}

void Application::onEvent(const WindowClosedEvent &event)
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

    //Triggers all subscribed to the start signal
    SystemSignals::s_startSignal();

    while (m_running)
    {
        delta = m_simulation.getDeltaTime();
        m_simulation.process();
        m_input.resetInputState();
        m_windowContext.pollEvents();

        //Triggers all queued events
        EBus::update<WindowResizedEvent>();
        EBus::update();

        //Triggers all subscribed to the update signal
        SystemSignals::s_updateSignal(delta.count());

        m_rendering.onRender();

        //Forces the Frame Rate
        std::this_thread::sleep_until(m_simulation.getLastTime() + m_desiredDelta);
    }

    //Triggers all subscribers to the finish signal
    SystemSignals::s_finishSignal();

    return 0;
}

void Application::setMaxFPS(std::uint16_t fps)
{
    m_desiredDelta = physics::delta_t{ 1.0f / float(fps) };
}

} // namespace spatial::core