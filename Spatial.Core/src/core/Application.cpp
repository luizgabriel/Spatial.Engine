#include <iostream>
#include <spatial/core/Application.h>
#include <spatial/desktop/PlatformEvent.h>

#include <chrono>
#include <thread>

using namespace spatial::common;
using namespace spatial::desktop;
using namespace std::chrono_literals;

namespace chr = std::chrono;

namespace spatial::core
{

Application::Application() : m_running{false}, m_desiredDelta{1.0f / 60.0f}, m_ebus{}, m_clock{}, m_windowContext{}
{
	m_ebus.connect<WindowClosedEvent>(this);
}

Application::~Application()
{
	m_ebus.disconnect<WindowClosedEvent>(this);
}

void Application::onEvent(const WindowClosedEvent& event)
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

	m_startSignal.trigger();

	while (m_running)
	{
		auto delta = m_clock.getDeltaTime().count();

		m_windowContext.pollEvents(m_ebus);

		m_frameStartSignal.trigger(delta);

		// Triggers all queued events
		m_ebus.update<WindowResizedEvent>();
		m_ebus.update();

		m_updateSignal.trigger(delta);

		m_updateGuiSignal.trigger(delta);

		m_frameEndSignal.trigger(delta);

		// Forces the Frame Rate
		std::this_thread::sleep_for(10ms);

		m_clock.tick();
	}

	m_finishSignal.trigger();

	return 0;
}

void Application::setMaxFPS(std::uint16_t fps)
{
	m_desiredDelta = chr::duration<float>{1.0f / fps};
}

} // namespace spatial::core