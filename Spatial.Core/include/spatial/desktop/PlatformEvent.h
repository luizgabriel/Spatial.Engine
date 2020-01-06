#pragma once

#include <spatial/common/Key.h>
#include <spatial/common/KeyAction.h>

namespace spatial::desktop
{

struct WindowResizedEvent
{
    int width, height;

    WindowResizedEvent(int width, int height) : width{width}, height{height} {}

    float getRatio() const
    {
        return float(width) / float(height);
    }
};

struct WindowClosedEvent
{
};

struct KeyEvent
{
    common::Key key;
    common::KeyAction action;
    int repeat;

    explicit KeyEvent(common::Key key, common::KeyAction action = common::KeyAction::None, int repeat = 0)
        : key{key}, action{action}, repeat{repeat}
    {
    }
};

struct MouseMovedEvent
{
    double x, y;
    MouseMovedEvent(double x, double y) : x{x}, y{y} {}
};

struct MouseScrolledEvent
{
    double xOffset, yOffset;

    MouseScrolledEvent(double x, double y) : xOffset{x}, yOffset{y} {}
};

struct MouseButtonEvent
{
    common::Key button;
    common::KeyAction action;

    MouseButtonEvent(common::Key button, common::KeyAction action) : button{button}, action{action} {}
};

} // namespace spatial::desktop