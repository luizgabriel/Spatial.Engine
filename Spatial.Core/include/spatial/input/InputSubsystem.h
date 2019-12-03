#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

class InputSubsystem
{
private:
    desktop::WindowContext &m_windowContext;

public:
    InputSubsystem(desktop::WindowContext &windowContext);
    ~InputSubsystem();

    void onMouseButtonEvent(const desktop::MouseButtonEvent &event);
    void onKeyEvent(const desktop::KeyEvent &event);
};

} // namespace spatial
