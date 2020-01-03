#include <spatial/input/InputSubsystem.h>
#include <spatial/common/EBus.h>
#include <spatial/input/Input.h>

namespace spatial::input
{

using namespace common;

InputSubsystem::InputSubsystem()
{
    EBus::connect<desktop::MouseButtonEvent>(this);
    EBus::connect<desktop::KeyEvent>(this);
}

InputSubsystem::~InputSubsystem()
{
    EBus::disconnect<desktop::MouseButtonEvent>(this);
    EBus::disconnect<desktop::KeyEvent>(this);
}

void InputSubsystem::resetInputState()
{
    Input::reset();
}

void InputSubsystem::onEvent(const desktop::MouseButtonEvent &event)
{
    Input::set(event.button, event.action);
}

void InputSubsystem::onEvent(const desktop::KeyEvent &event)
{
    Input::set(event.key, event.action);
}

} // namespace spatial
