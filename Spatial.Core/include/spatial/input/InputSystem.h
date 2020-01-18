#pragma once

#include <spatial/core/Application.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/Key.h>
#include <spatial/common/KeyAction.h>

namespace spatial::input
{

class InputSystem
{
public:
    InputSystem(core::Application& app);

    void onStartFrame(float delta);

    void onEvent(const desktop::MouseMovedEvent &event);
    void onEvent(const desktop::KeyEvent &event);
    void onEvent(const desktop::TextEvent &event);
};

} // namespace spatial
