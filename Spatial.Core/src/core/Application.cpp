#include <spatial/core/Application.h>
#include <spatial/desktop/PlatformEvent.h>

#include <chrono>
#include <thread>


using namespace std::chrono_literals;

namespace spatial
{

Application::Application(const Configuration& config)
	: m_window{[this, &config]()
	  {
		  const auto windowTitle = config.get<std::string, 2>("window.title");
		  const auto windowWidth = config.get<std::uint16_t, 2>("window.width");
		  const auto windowHeight = config.get<std::uint16_t, 2>("window.height");

		  return this->m_windowContext.createWindow(windowWidth, windowHeight, windowTitle);
	  }()},
	  m_rendering{m_window.getNativeHandle()},
	  m_ui{[this, &config]()
	  {
		  const auto fontPath = config.get<std::string, 2>("ui.font-path");
		  return UserInterfaceSystem{this->m_rendering.getEngine(), fontPath};
	  }()}
{
	m_ebus.connect<WindowClosedEvent>(this);
	
	m_input.attach(m_ebus);
	m_rendering.attach(m_ebus);
	m_ui.attach(m_ebus);

	m_rendering.setupViewport(m_window.getFrameBufferSize());
	m_ui.setupViewport(m_window.getWindowSize(), m_window.getFrameBufferSize());

	m_rendering.pushFrontView(m_ui.getView());
}

Application::~Application()
{
	m_ebus.disconnect<WindowClosedEvent>(this);
	
	m_input.detach(m_ebus);
	m_rendering.detach(m_ebus);
	m_ui.detach(m_ebus);
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

	m_rendering.onStart();
	m_ui.onStart();

	m_startSignal();
	
	while (m_running)
	{
		const auto delta = m_clock.getDeltaTime().count();

		m_input.onStartFrame(delta);
		m_ui.onStartFrame(delta);

		m_windowContext.pollEvents(m_ebus);

		// Triggers all queued events
		m_ebus.update<WindowResizedEvent>();
		m_ebus.update();

		m_updateFrameSignal(delta);

		m_drawGuiSignal();

		m_ui.onEndGuiFrame();
		m_rendering.onEndFrame();

		std::this_thread::sleep_until(m_clock.getLastTime() + m_desiredDelta);

		m_clock.tick();
	}

	m_finishSignal();

	return 0;
}

void Application::setMaxFps(float fps)
{
	m_desiredDelta = delta_t{1.0f / fps};
}
} // namespace spatial::core
