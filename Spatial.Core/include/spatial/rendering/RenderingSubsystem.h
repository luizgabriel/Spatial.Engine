#pragma once

#include <spatial/desktop/Window.h>

namespace spatial
{

class RenderingSubsystem
{
private:
    desktop::WindowContext m_windowContext;
    desktop::Window m_mainWindow;

public:
    RenderingSubsystem();

    void pollWindowEvents();
    void onStartFrame();
    void onEndFrame();

    desktop::Window* getWindow() { return &m_mainWindow; }
    desktop::WindowContext* getWindowContext() { return &m_windowContext; }
};

} // namespace spatial