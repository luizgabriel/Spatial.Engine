#include <spatial/input/InputSubsystem.h>
#include <spatial/core/EBus.h>
#include <spatial/input/Input.h>

using namespace spatial::core;
using namespace spatial::desktop;

namespace spatial::input
{

InputSubsystem::InputSubsystem()
{
    EBus::connect<MouseMovedEvent>(this);
    EBus::connect<KeyEvent>(this);
}

InputSubsystem::~InputSubsystem()
{
    EBus::disconnect<MouseMovedEvent>(this);
    EBus::disconnect<KeyEvent>(this);
}

void InputSubsystem::resetInputState()
{
    Input::s_inputState.reset();
}

void InputSubsystem::onEvent(const MouseMovedEvent &event)
{
    Input::s_inputState.setMousePosition({event.x, event.y});
}

void InputSubsystem::onEvent(const KeyEvent &event)
{
    Input::s_inputState.set(event.key, event.action);
}

} // namespace spatial::input
