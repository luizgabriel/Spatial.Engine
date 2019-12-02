#pragma once

#include <spatial/desktop/Window.h>

namespace spatial {

class RenderingSubsystem {
private:
    WindowContext m_windowContext;
    Window m_mainWindow;

public:
    RenderingSubsystem();
    
    void pollWindowEvents();
    void onStartFrame();
    void onUpdateFrame();
    void onEndFrame();

    const Window& getWindow() { return m_mainWindow; }
};

}