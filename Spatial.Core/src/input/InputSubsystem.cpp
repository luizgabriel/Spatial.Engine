#include <spatial/input/InputSubsystem.h>
#include <spatial/input/Input.h>
#include <spatial/common/Key.h>
#include <spatial/common/KeyAction.h>
#include <imgui.h>

using namespace spatial::core;
using namespace spatial::desktop;
using namespace spatial::common;

namespace spatial::input
{

InputSubsystem::InputSubsystem(Application* app)
    : m_mouseEventConnector{app, this},
      m_keyEventConnector{app, this},
      m_textEventConnector{app, this},
      m_signalsConnector{app, this}
{
}

void InputSubsystem::onStart()
{
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.KeyMap[ImGuiKey_Tab] = (int) Key::Tab;
    io.KeyMap[ImGuiKey_LeftArrow] = (int) Key::Left;
    io.KeyMap[ImGuiKey_RightArrow] = (int) Key::Right;
    io.KeyMap[ImGuiKey_UpArrow] = (int) Key::Up;
    io.KeyMap[ImGuiKey_DownArrow] = (int) Key::Down;
    io.KeyMap[ImGuiKey_PageUp] = (int) Key::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = (int) Key::PageDown;
    io.KeyMap[ImGuiKey_Home] = (int) Key::Home;
    io.KeyMap[ImGuiKey_End] = (int) Key::End;
    io.KeyMap[ImGuiKey_Insert] = (int) Key::Insert;
    io.KeyMap[ImGuiKey_Delete] = (int) Key::Delete;
    io.KeyMap[ImGuiKey_Backspace] = (int) Key::Backspace;
    io.KeyMap[ImGuiKey_Space] = (int) Key::Space;
    io.KeyMap[ImGuiKey_Enter] = (int) Key::Enter;
    io.KeyMap[ImGuiKey_Escape] = (int) Key::Escape;
    io.KeyMap[ImGuiKey_A] = (int) Key::A;
    io.KeyMap[ImGuiKey_C] = (int) Key::C;
    io.KeyMap[ImGuiKey_V] = (int) Key::V;
    io.KeyMap[ImGuiKey_X] = (int) Key::X;
    io.KeyMap[ImGuiKey_Y] = (int) Key::Y;
    io.KeyMap[ImGuiKey_Z] = (int) Key::Z;
}

void InputSubsystem::onStartFrame(float delta)
{
    Input::s_inputState.reset();
}

void InputSubsystem::onEvent(const MouseMovedEvent &event)
{
    Input::s_inputState.setMousePosition({event.x, event.y});

    auto &io = ImGui::GetIO();
    io.MousePos = ImVec2(event.x, event.y);
}

void InputSubsystem::onEvent(const KeyEvent &event)
{
    auto key = event.key;
    Input::s_inputState.set(key, event.action);

    auto pressed = event.action == KeyAction::Pressed;
    auto &io = ImGui::GetIO();
    io.MouseDown[0] = key == Key::MouseLeft && pressed;
    io.MouseDown[1] = key == Key::MouseRight && pressed;
    io.MouseDown[2] = key == Key::MouseMiddle && pressed;

    io.KeysDown[(int) key] = pressed;
    io.KeyCtrl = (key == Key::LControl || key == Key::RControl) && pressed;
    io.KeyShift = (key == Key::LShift || key == Key::RShift) && pressed;
    io.KeyAlt = (key == Key::LAlt || key == Key::RAlt) && pressed;
    io.KeySuper = (key == Key::System) && pressed;
}

void InputSubsystem::onEvent(const TextEvent &event)
{
    auto &io = ImGui::GetIO();
    io.AddInputCharactersUTF8(event.text.c_str());
}

} // namespace spatial::input
