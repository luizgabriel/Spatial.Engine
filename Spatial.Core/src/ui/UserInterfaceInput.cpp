#include <spatial/ui/UserInterfaceInput.h>
#include <imgui.h>

using namespace filament::math;

namespace spatial
{

UserInterfaceInput::UserInterfaceInput()
{
}

void UserInterfaceInput::setup()
{
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	io.KeyMap[ImGuiKey_Tab] = static_cast<int>(Key::Tab);
	io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Key::Left);
	io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Key::Right);
	io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Key::Up);
	io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Key::Down);
	io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(Key::PageUp);
	io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(Key::PageDown);
	io.KeyMap[ImGuiKey_Home] = static_cast<int>(Key::Home);
	io.KeyMap[ImGuiKey_End] = static_cast<int>(Key::End);
	io.KeyMap[ImGuiKey_Insert] = static_cast<int>(Key::Insert);
	io.KeyMap[ImGuiKey_Delete] = static_cast<int>(Key::Delete);
	io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(Key::Backspace);
	io.KeyMap[ImGuiKey_Space] = static_cast<int>(Key::Space);
	io.KeyMap[ImGuiKey_Enter] = static_cast<int>(Key::Enter);
	io.KeyMap[ImGuiKey_Escape] = static_cast<int>(Key::Escape);
	io.KeyMap[ImGuiKey_A] = static_cast<int>(Key::A);
	io.KeyMap[ImGuiKey_C] = static_cast<int>(Key::C);
	io.KeyMap[ImGuiKey_V] = static_cast<int>(Key::V);
	io.KeyMap[ImGuiKey_X] = static_cast<int>(Key::X);
	io.KeyMap[ImGuiKey_Y] = static_cast<int>(Key::Y);
	io.KeyMap[ImGuiKey_Z] = static_cast<int>(Key::Z);
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

	io.KeysDown[static_cast<int>(key)] = pressed;
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