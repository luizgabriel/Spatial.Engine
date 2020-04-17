#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/common/Signal.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Logger.h>
#include <spatial/core/Clock.h>
#include <spatial/desktop/PlatformEvent.h>

namespace spatial
{

class Application final
{
private:
	bool m_running = false;

	float m_desiredDelta{1.0f / 60.0f};

	EventQueue m_ebus{};
	Clock m_clock{};

	WindowContext m_windowContext{};

	// region Signals
	Signal<> m_startSignal;
	Signal<float> m_startFrameSignal;
	Signal<float> m_updateFrameSignal;
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

	bool isRunning() const { return m_running; }

	void onEvent(const WindowClosedEvent& event);

	void setMaxFps(float fps);

	// region Getters
	/**
	 * \brief Called when the application is start by `run()`
	 */
	auto& getStartSignal() { return m_startSignal; }

	/**
	 * \brief Called every start of frame
	 */
	auto& getStartFrameSignal() { return m_startFrameSignal; }

	/**
	 * \brief Called every update of frame
	 */
	auto& getUpdateFrameSignal() { return m_updateFrameSignal; }

	/**
	 * \brief Called every update of frame
	 */
	auto& getEndGuiFrameSignal() { return m_endGuiFrameSignal; }

	/**
	 * \brief Called every end of frame
	 */
	auto& getEndFrameSignal() { return m_endFrameSignal; }

	/**
	 * \brief Called when the application is closed
	 */
	auto& getFinishSignal() { return m_finishSignal; }

	/**
	 * \brief Gets the main application event bus channel
	 */
	auto& getEBus()  { return m_ebus; }

	/**
	 * \brief Gets the window context
	 * The window context is responsible for creating windows.
	 * Its lifetime should last longer than all windows
	 */
	const auto& getWindowContext() const { return m_windowContext; }
	// endregion Getters
};

} // namespace spatial::core
