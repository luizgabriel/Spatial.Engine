#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <spatial/input/InputState.h>

namespace spatial::input
{

class InputSubsystem
{
public:
    InputSubsystem();
    ~InputSubsystem();

    void resetInputState();

    void onEvent(const desktop::MouseButtonEvent &event);
    void onEvent(const desktop::MouseMovedEvent &event);
    void onEvent(const desktop::KeyEvent &event);

};

} // namespace spatial
