#include <spatial/ui/components/ImGuiWindow.h>

namespace spatial::ui {

ImGuiWindow::ImGuiWindow(const std::string_view name)
{
	ImGui::SetNextWindowSize(ImVec2(0, 0));
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