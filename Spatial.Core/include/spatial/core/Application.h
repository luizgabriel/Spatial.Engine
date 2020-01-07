#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/render/RenderingSubsystem.h>
#include <spatial/input/InputSubsystem.h>
#include <spatial/physics/SimulationSubsystem.h>
#include <spatial/common/Signal.h>

namespace spatial::core
{

class Application final
{
private:
    bool m_running;

    physics::delta_t m_desiredDelta;

    desktop::WindowContext m_windowContext;

    //region Subsystems
    input::InputSubsystem m_input;
    render::RenderingSubsystem m_rendering;
    physics::SimulationSubsystem m_simulation;
    //endregion

public:

    //region Signals
    common::Signal<> onStartSignal;
    common::Signal<float> onUpdateSignal;
    common::Signal<> onFinishSignal;
    //endregion

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
    render::RenderingSubsystem &getRenderSys()
    {
        return m_rendering;
    }

    const render::RenderingSubsystem &getRenderSys() const
    {
        return m_rendering;
    }
    //endregion Getters
};

} // namespace spatial::core
