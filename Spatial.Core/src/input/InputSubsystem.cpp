#include <spatial/input/InputSubsystem.h>
#include <iostream>
#include <fmt/format.h>

namespace spatial
{

InputSubsystem::InputSubsystem(desktop::WindowContext &windowContext)
    : m_windowContext{windowContext}
{
    m_windowContext.connect<desktop::MouseButtonEvent, &InputSubsystem::onMouseButtonEvent>(this);
    m_windowContext.connect<desktop::KeyEvent, &InputSubsystem::onKeyEvent>(this);
}

InputSubsystem::~InputSubsystem()
{
    m_windowContext.disconnect<desktop::MouseButtonEvent, &InputSubsystem::onMouseButtonEvent>(this);
    m_windowContext.disconnect<desktop::KeyEvent, &InputSubsystem::onKeyEvent>(this);
}

void InputSubsystem::onMouseButtonEvent(const desktop::MouseButtonEvent &event)
{
    std::cout << fmt::format("MOUSE BUTTON: {0} [{1}]\n", event.button, nameOf(event.action));
    //TODO: Update global mouse input state
}

void InputSubsystem::onKeyEvent(const desktop::KeyEvent &event)
{
    auto key = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(event.scanCode));
    std::cout << fmt::format("KEY: {0} ({1}) [{2}]\n", static_cast<char>(key), event.scanCode, nameOf(event.action));
    //TODO: Update global key input state
}

} // namespace spatial
