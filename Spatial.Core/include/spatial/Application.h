#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/rendering/RenderingSubsystem.h>
#include <spatial/input/InputSubsystem.h>

namespace spatial
{

class Application
{
private:
    desktop::WindowContext m_windowContext;
    desktop::Window m_mainWindow;

    //region Subsystems

    RenderingSubsystem m_rendering;
    InputSubsystem m_input;

    //endregion

public:
    Application();

    int run();
};

} // namespace spatial
