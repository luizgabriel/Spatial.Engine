#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/Key.h>
#include <spatial/common/KeyAction.h>
#include <spatial/common/EventQueue.h>

namespace spatial::input
{

class InputSystem
{
public:
    InputSystem();

    InputSystem(const InputSystem &other) = delete;
    InputSystem(InputSystem &&other) = default;

    void attach(common::EventQueue &queue);
    void detach(common::EventQueue &queue);

    void onStartFrame(float delta);

    void onEvent(const desktop::MouseMovedEvent &event);
    void onEvent(const desktop::KeyEvent &event);
    void onEvent(const desktop::TextEvent &event);
};

} // namespace spatial::input
