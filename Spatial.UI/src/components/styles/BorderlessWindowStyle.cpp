#include <imgui.h>
#include <spatial/ui/components/styles/BorderlessWindowStyle.h>

namespace spatial::ui
{

BorderlessWindowStyle::BorderlessWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}

BorderlessWindowStyle::~BorderlessWindowStyle()
{
	ImGui::PopStyleVar(3);
}

} // namespace spatial::ui