#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/render/RenderingSubsystem.h>
#include <spatial/input/InputSubsystem.h>

namespace spatial
{

class Application
{
private:
    bool m_running;

    desktop::WindowContext m_windowContext;
    desktop::Window m_mainWindow;

    //region Subsystems

    InputSubsystem m_input;
    render::RenderingSubsystem m_rendering;

    //endregion
    void onWindowClosed(const desktop::WindowClosedEvent& event);

public:
    Application();

    int run();

    void stop();

    bool isRunning() const { return m_running; }
};

} // namespace spatial
