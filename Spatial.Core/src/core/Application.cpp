#include <spatial/core/Application.h>
#include <chrono>
#include <thread>
#include <spatial/core/Logger.h>

using namespace std::chrono_literals;

namespace spatial
{

Application::Application()
{
	m_eventQueue.connect<WindowClosedEvent>(*this);
}

Application::~Application()
{
	m_eventQueue.disconnect<WindowClosedEvent>(*this);
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

		m_windowContext.pollEvents(m_eventQueue);

		m_startFrameSignal(delta);

		// Triggers all queued events
		m_eventQueue.update<WindowResizedEvent>();
		m_eventQueue.update();

		while (m_clock.hasLag(m_desiredDelta))
		{
			m_updateFrameSignal(m_desiredDelta);
			m_clock.fixLag();
		}

		m_drawGuiSignal();

		m_endGuiFrameSignal();
		m_endFrameSignal();

		std::this_thread::sleep_until(m_clock.getLastTime() + Clock<float>::delta_t{m_desiredDelta});

		m_clock.tick();
	}

	m_finishSignal();

	return 0;
}

void Application::setMaxFps(float fps)
{
	m_desiredDelta = 1.0f / fps;
}

} // namespace spatial
