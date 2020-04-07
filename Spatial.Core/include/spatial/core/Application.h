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

	std::chrono::duration<float> m_desiredDelta{1.0f / 60.0f};

	EventQueue m_ebus{};
	Clock m_clock{};

	WindowContext m_windowContext{};

	// region Signals
	Signal<> m_startSignal;
	Signal<float> m_frameStartSignal;
	Signal<float> m_updateSignal;
	Signal<float> m_updateGuiSignal;
	Signal<float> m_frameEndSignal;
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
	auto& getFrameStartSignal() { return m_frameStartSignal; }

	/**
	 * \brief Called every update of frame
	 */
	auto& getUpdateSignal() { return m_updateSignal; }

	/**
	 * \brief Called every update of frame
	 */
	auto& getUpdateGuiSignal() { return m_updateGuiSignal; }

	/**
	 * \brief Called every end of frame
	 */
	auto& getFrameEndSignal() { return m_frameEndSignal; }

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
