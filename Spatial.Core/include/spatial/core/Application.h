#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/common/Signal.h>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Logger.h>
#include <spatial/core/Clock.h>
#include <spatial/desktop/PlatformEvent.h>

namespace spatial::core
{

class Application final
{
private:
    bool m_running;

    std::chrono::duration<float> m_desiredDelta;

    common::EventQueue m_ebus;
    spdlog::logger m_logger;
    Clock m_clock;

    desktop::WindowContext m_windowContext;

    //region Signals
    common::Signal<> m_startSignal;

    common::Signal<float> m_frameStartSignal;
    common::Signal<float> m_updateSignal;
    common::Signal<float> m_frameEndSignal;

    common::Signal<> m_finishSignal;
    //endregion

public:
    Application();
    ~Application();

    Application(const Application &other) = delete;
    Application &operator=(const Application &other) = delete;

    int run();

    void stop();

    bool isRunning() const { return m_running; }

    void onEvent(const desktop::WindowClosedEvent &event);

    void setMaxFPS(std::uint16_t fps);

    //region Getters
    /**
     * \brief Called when the application is start by `run()`
     */
    auto &getStartSignal() { return m_startSignal; }

    /**
     * \brief Called every start of frame
     */
    auto &getFrameStartSignal() { return m_frameStartSignal; }

    /**
     * \brief Called every update of frame
     */
    auto &getUpdateSignal() { return m_updateSignal; }

    /**
     * \brief Called every end of frame
     */
    auto &getFrameEndSignal() { return m_frameEndSignal; }

    /**
     * \brief Called when the application is closed
     */
    auto &getFinishSignal() { return m_finishSignal; }

    /**
     * \brief Gets the main application event bus channel
     */
    auto &getEBus() { return m_ebus; }

    /**
     * \brief Gets the main logger 
     */
    auto &getLogger() { return m_logger; }

    /**
     * \brief Gets the window context
     * The window context is responsible for creating windows.
     * Its lifetime should last longer than all windows
     */
    auto& getWindowContext() { return m_windowContext; }
    //endregion Getters
};

} // namespace spatial::core
