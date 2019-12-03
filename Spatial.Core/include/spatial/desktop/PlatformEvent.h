#pragma once

#include <spatial/common/KeyAction.h>

namespace spatial::desktop
{

struct WindowResizedEvent
{
    int width, height;

    WindowResizedEvent(int width, int height) : width{width}, height{height} {}
};
struct WindowClosedEvent
{
};
struct KeyEvent
{
    int key;
    common::KeyAction action;

    explicit KeyEvent(int key, common::KeyAction action = common::KeyAction::None) : key{key}, action{action} {}
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