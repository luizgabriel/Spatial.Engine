#include <spatial/core/Application.h>
#include <spatial/desktop/PlatformEvent.h>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace spatial
{

Application::Application()
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

	m_startSignal();

	while (m_running)
	{
		const auto delta = m_clock.getDeltaTime().count();

		m_windowContext.pollEvents(m_ebus);

		m_startFrameSignal(delta);

		// Triggers all queued events
		m_ebus.update<WindowResizedEvent>();
		m_ebus.update();

		m_updateFrameSignal(delta);

		m_endGuiFrameSignal();
		m_endFrameSignal();

		//std::this_thread::sleep_until(m_clock.getLastTime() + delta_t{m_desiredDelta});
		std::this_thread::sleep_for(10ms);
		
		m_clock.tick();
	}

	m_finishSignal();

	return 0;
}

void Application::setMaxFps(float fps)
{
	m_desiredDelta = 1.0f / fps;
}

} // namespace spatial::core