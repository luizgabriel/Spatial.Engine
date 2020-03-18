#include <spatial/ui/UserInterfaceInput.h>
#include <imgui.h>

using namespace filament::math;
using namespace spatial::common;

namespace spatial::ui
{

UserInterfaceInput::UserInterfaceInput()
{
}

void UserInterfaceInput::setup()
{
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	io.KeyMap[ImGuiKey_Tab] = (int)Key::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = (int)Key::Left;
	io.KeyMap[ImGuiKey_RightArrow] = (int)Key::Right;
	io.KeyMap[ImGuiKey_UpArrow] = (int)Key::Up;
	io.KeyMap[ImGuiKey_DownArrow] = (int)Key::Down;
	io.KeyMap[ImGuiKey_PageUp] = (int)Key::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = (int)Key::PageDown;
	io.KeyMap[ImGuiKey_Home] = (int)Key::Home;
	io.KeyMap[ImGuiKey_End] = (int)Key::End;
	io.KeyMap[ImGuiKey_Insert] = (int)Key::Insert;
	io.KeyMap[ImGuiKey_Delete] = (int)Key::Delete;
	io.KeyMap[ImGuiKey_Backspace] = (int)Key::Backspace;
	io.KeyMap[ImGuiKey_Space] = (int)Key::Space;
	io.KeyMap[ImGuiKey_Enter] = (int)Key::Enter;
	io.KeyMap[ImGuiKey_Escape] = (int)Key::Escape;
	io.KeyMap[ImGuiKey_A] = (int)Key::A;
	io.KeyMap[ImGuiKey_C] = (int)Key::C;
	io.KeyMap[ImGuiKey_V] = (int)Key::V;
	io.KeyMap[ImGuiKey_X] = (int)Key::X;
	io.KeyMap[ImGuiKey_Y] = (int)Key::Y;
	io.KeyMap[ImGuiKey_Z] = (int)Key::Z;
}

void UserInterfaceInput::setMousePosition(float2 position)
{
	auto& io = ImGui::GetIO();
	io.MousePos = ImVec2(position.x, position.y);
}

void UserInterfaceInput::setKey(Key key, KeyAction action)
{
	auto pressed = action == KeyAction::Pressed;
	auto& io = ImGui::GetIO();
	io.MouseDown[0] = key == Key::MouseLeft && pressed;
	io.MouseDown[1] = key == Key::MouseRight && pressed;
	io.MouseDown[2] = key == Key::MouseMiddle && pressed;

	io.KeysDown[(int)key] = pressed;
	io.KeyCtrl = (key == Key::LControl || key == Key::RControl) && pressed;
	io.KeyShift = (key == Key::LShift || key == Key::RShift) && pressed;
	io.KeyAlt = (key == Key::LAlt || key == Key::RAlt) && pressed;
	io.KeySuper = (key == Key::System) && pressed;
}

void UserInterfaceInput::setText(const std::string& text)
{
	auto& io = ImGui::GetIO();
	io.AddInputCharactersUTF8(text.c_str());
}

} // namespace spatial::ui