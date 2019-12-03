#pragma once

namespace spatial::desktop
{

struct WindowResized
{
    int width, height;

    WindowResized(int width, int height) : width{width}, height{height} {}
};

struct WindowClosed
{
};

struct KeyTyped
{
    int key, action;
    KeyTyped(int key, int action) : key{key}, action{action} {}
};

struct MouseMoved
{
    double x, y;
    MouseMoved(double x, double y) : x{x}, y{y} {}
};

struct MouseScrolled
{
    double xOffset, yOffset;

    MouseScrolled(double x, double y) : xOffset{x}, yOffset{y} {}
};

struct MouseButton
{
    int button, action;
    MouseButton(int button, int action) : button{button}, action{action} {}
};

} // namespace spatial::desktop