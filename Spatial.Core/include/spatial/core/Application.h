#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/common/Signal.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Clock.h>
#include <spatial/desktop/PlatformEvent.h>

namespace spatial
{

class Application final
{
private:
	bool m_running = false;

	EventQueue m_eventQueue{};
	Clock<float> m_clock{};
	float m_desiredDelta{1.0f / 60.0f};

	WindowContext m_windowContext;

	// region Signals
	Signal<> m_startSignal;

	Signal<float> m_startFrameSignal;
	Signal<float> m_updateFrameSignal;
	Signal<> m_drawGuiSignal;
	Signal<> m_endGuiFrameSignal;
	Signal<> m_endFrameSignal;

	Signal<> m_finishSignal;
	// endregion

public:
	Application();
	~Application();

	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;

	int run();

	void stop();

	bool isRunning() const
	{
		return m_running;
	}

	void onEvent(const WindowClosedEvent& event);

	void setMaxFps(float fps);

	auto& getEventQueue()
	{
		return m_eventQueue;
	}

	auto& getWindowContext() const
	{
		return m_windowContext;
	}

	auto& getStartSignal()
	{
		return m_startSignal;
	}

	auto& getFinishSignal()
	{
		return m_finishSignal;
	}

	auto& getStartFrameSignal()
	{
		return m_startFrameSignal;
	}

	auto& getUpdateFrameSignal()
	{
		return m_updateFrameSignal;
	}

	auto& getEndGuiFrameSignal()
	{
		return m_endGuiFrameSignal;
	}

	auto& getEndFrameSignal()
	{
		return m_endFrameSignal;
	}

	auto& getDrawGuiSignal()
	{
		return m_drawGuiSignal;
	}
};

} // namespace spatial
