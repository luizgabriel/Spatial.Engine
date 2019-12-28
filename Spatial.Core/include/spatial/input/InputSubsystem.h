#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial::input
{

class InputSubsystem
{
public:
    InputSubsystem();
    ~InputSubsystem();

    void onEvent(const desktop::MouseButtonEvent &event);
    void onEvent(const desktop::KeyEvent &event);
};

} // namespace spatial
