#include <spatial/input/InputSystem.h>
#include <spatial/input/Input.h>
#include <spatial/core/ApplicationEvents.h>

using namespace spatial::core;
using namespace spatial::desktop;
using namespace spatial::common;

namespace spatial::input
{

InputSystem::InputSystem(Application& app)
{
    connect(app, this);
    connect<MouseMovedEvent>(app, this);
    connect<KeyEvent>(app, this);
    connect<TextEvent>(app, this);
}

void InputSystem::onStartFrame(float delta)
{
    Input::s_inputState.reset();
}

void InputSystem::onEvent(const MouseMovedEvent &event)
{
    Input::s_inputState.setMousePosition({event.x, event.y});
}

void InputSystem::onEvent(const KeyEvent &event)
{
    auto key = event.key;
    Input::s_inputState.set(key, event.action);
}

void InputSystem::onEvent(const TextEvent &event)
{
}

} // namespace spatial::input
