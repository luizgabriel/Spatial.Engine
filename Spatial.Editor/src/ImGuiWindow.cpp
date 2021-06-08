#include "ImGuiWindow.h"

namespace spatial::editor {

ImGuiWindow::ImGuiWindow(const std::string_view name)
{
	ImGui::Begin(name.data());
}

math::float2 ImGuiWindow::getSize()
{
	auto size = ImGui::GetWindowSize();
	return {size.x, size.y};
}

ImGuiWindow::~ImGuiWindow()
{
	ImGui::End();
}

}