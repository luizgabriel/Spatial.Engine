#include <spatial/ui/components/styles/NoPaddingWindow.h>
#include <imgui.h>

namespace spatial::ui
{

NoPaddingWindow::NoPaddingWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

NoPaddingWindow::~NoPaddingWindow()
{
	ImGui::PopStyleVar();
}

}