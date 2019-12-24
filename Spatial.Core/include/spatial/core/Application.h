#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/common/Event.h>
#include <spatial/render/RenderingSubsystem.h>
#include <spatial/input/InputSubsystem.h>
#include <spatial/physics/SimulationSubsystem.h>

namespace spatial
{
class Application final
{
private:
    bool m_running;

    desktop::WindowContext m_windowContext;

    //region Subsystems
    InputSubsystem m_input;
    render::RenderingSubsystem m_rendering;
    physics::SimulationSubsystem m_simulation;
    //endregion

    void onWindowClosed(const desktop::WindowClosedEvent& event);

public:
    Application();

    int run();

    void stop();

    bool isRunning() const { return m_running; }

     //region Events
    common::Event<float> onUpdateEvent;
    common::Event<> onStartEvent;
    common::Event<> onFinishEvent;
    //endregion

    const desktop::WindowContext* getWindowContext()
    {
        return &m_windowContext;
    }
};

} // namespace spatial
