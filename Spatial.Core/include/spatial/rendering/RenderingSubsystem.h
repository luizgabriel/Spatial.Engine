#pragma once

#include <spatial/desktop/Window.h>

namespace spatial
{

class RenderingSubsystem
{
private:
    desktop::Window &m_mainWindow;

public:
    RenderingSubsystem(desktop::Window &window);

    void onStartFrame();
    void onEndFrame();
};

} // namespace spatial