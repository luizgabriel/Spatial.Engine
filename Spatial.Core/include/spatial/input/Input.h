#pragma once

#include <spatial/input/InputState.h>

namespace spatial::input
{

class InputSubsystem;

class Input
{
private:
    static InputState &state();
    static void set(common::Key key, common::KeyAction action);
    static void reset();

public:
    static bool pressed(common::Key key);
    static bool released(common::Key key);
    static filament::math::float2 mouse();

    friend class InputSubsystem;
};

} // namespace spatial::input