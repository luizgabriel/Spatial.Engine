#pragma once

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
    int scanCode;
    int repeat;
    common::KeyAction action;

    explicit KeyEvent(int scanCode, int repeat = 0, common::KeyAction action = common::KeyAction::None) 
        : scanCode{scanCode}, repeat{repeat}, action{action} {}
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
    int button;
    common::KeyAction action;

    MouseButtonEvent(int button, common::KeyAction action) : button{button}, action{action} {}
};

} // namespace spatial::desktop