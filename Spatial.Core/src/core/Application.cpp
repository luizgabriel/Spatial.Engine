#include <iostream>
#include <spatial/core/Application.h>
#include <spatial/core/EBus.h>
#include <spatial/desktop/PlatformEvent.h>

#include <chrono>
#include <thread>

using namespace spatial::common;
using namespace spatial::desktop;
using namespace std::chrono_literals;

namespace chr = std::chrono;

namespace spatial::core
{

Application::Application()
    : m_running{false},
      m_desiredDelta{1.0f / 60.0f},
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
    m_running = true;

    m_input.onStart();
    m_rendering.onStart();
    
    onStartSignal();

    while (m_running)
    {
        m_simulation.process();
        m_input.resetInputState();
        m_windowContext.pollEvents();

        auto delta = m_simulation.getDeltaTime().count();

        m_rendering.beforeRender(delta);

        //Triggers all queued events
        EBus::update<WindowResizedEvent>();
        EBus::update();
        
        onUpdateSignal(delta);

        m_rendering.render();

        //Forces the Frame Rate
        std::this_thread::sleep_for(10ms);
    }

    onFinishSignal();

    return 0;
}

void Application::setMaxFPS(std::uint16_t fps)
{
    m_desiredDelta = physics::delta_t{ 1.0f / float(fps) };
}

} // namespace spatial::core