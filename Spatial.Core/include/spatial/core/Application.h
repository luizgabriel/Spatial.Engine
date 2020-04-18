#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/common/Signal.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Clock.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/input/InputSystem.h>
#include <spatial/ui/UserInterfaceSystem.h>
#include <toml++/toml.h>

namespace spatial
{

class Application final
{
private:
	bool m_running = false;
	delta_t m_desiredDelta{1.0f / 60.0f};

	EventQueue m_ebus{};
	Clock m_clock{};

	WindowContext m_windowContext;
	Window m_window;
	InputSystem m_input;
	RenderingSystem m_rendering;
	UserInterfaceSystem m_ui;

	// region Signals
	Signal<> m_startSignal;
	Signal<float> m_updateFrameSignal;
	Signal<> m_drawGuiSignal;
	Signal<> m_finishSignal;
	// endregion

public:
	Application(const toml::table& config);
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
	 * \brief Called every update of frame
	 */
	auto& getUpdateFrameSignal() { return m_updateFrameSignal; }

	/**
	 * \brief Called every update of frame
	 */
	auto& getDrawGuiSignal() { return m_drawGuiSignal; }

	/**
	 * \brief Called when the application is closed
	 */
	auto& getFinishSignal() { return m_finishSignal; }

	/**
	 * \brief Gets the main application event bus channel
	 */
	auto& getEBus()  { return m_ebus; }

	/*
	 * \brief Gets the main window
	 */
	auto& getWindow() { return m_window; }

	/*
	 * \brief Gets the main window
	 */
	auto& getInputSystem() { return m_input; }

	/*
	 * \brief Gets the main window
	 */
	auto& getRenderingSystem() { return m_rendering; }

	/*
	 * \brief Get user interface
	 */
	auto& getUserInterfaceSystem() { return m_input; }
	
	// endregion Getters
};

} // namespace spatial::core
