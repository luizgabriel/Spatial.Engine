#include <spatial/input/InputSubsystem.h>
#include <spatial/common/EBus.h>
#include <iostream>
#include <fmt/format.h>

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

void InputSubsystem::onEvent(const desktop::MouseButtonEvent &event)
{
    std::cout << fmt::format("MOUSE BUTTON: {0} [{1}]\n", event.button, nameOf(event.action));
    //TODO: Update global mouse input state
}

void InputSubsystem::onEvent(const desktop::KeyEvent &event)
{
    auto key = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(event.scanCode));
    std::cout << fmt::format("KEY: {0} ({1}) [{2}]\n", static_cast<char>(key), event.scanCode, nameOf(event.action));
    //TODO: Update global key input state
}

} // namespace spatial
