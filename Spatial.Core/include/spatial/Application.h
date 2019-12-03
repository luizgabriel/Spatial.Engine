#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/rendering/RenderingSubsystem.h>

namespace spatial
{

class Application
{
private:
    desktop::WindowContext m_windowContext;
    desktop::Window m_mainWindow;

    //region Subsystems

    RenderingSubsystem m_rendering;

    //endregion

public:
    Application();

    int run();
};

} // namespace spatial
