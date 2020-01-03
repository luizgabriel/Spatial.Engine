#include <spatial/input/Input.h>

namespace spatial::input
{

InputState &Input::state()
{
    static InputState state;
    return state;
}

void Input::set(common::Key key, common::KeyAction action)
{
    state().set(key, action);
}

void Input::reset()
{
    state().reset();
}

bool Input::pressed(common::Key key)
{
    return state().isPressed(key);
}

bool Input::released(common::Key key)
{
    return state().isReleased(key);
}

filament::math::float2 Input::mouse()
{
    return state().getMousePosition();
}

} // namespace spatial::input