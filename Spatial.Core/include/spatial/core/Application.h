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

    Application(const Application& other) = delete;
    Application &operator=(const Application &other) = delete;

    int run();

    void stop();

    bool isRunning() const { return m_running; }

     //region Events
    common::Event<float> onUpdateEvent;
    common::Event<float> onRenderEvent;
    common::Event<float> onGuiEvent;
    
    common::Event<> onStartEvent;
    common::Event<> onFinishEvent;
    //endregion

    desktop::WindowContext& getWindowContext()
    {
        return m_windowContext;
    }

    const desktop::WindowContext& getWindowContext() const
    {
        return m_windowContext;
    }

    render::RenderingSubsystem& getRenderSys()
    {
        return m_rendering;
    }

    const render::RenderingSubsystem& getRenderSys() const
    {
        return m_rendering;
    }
    
    
};

} // namespace spatial
