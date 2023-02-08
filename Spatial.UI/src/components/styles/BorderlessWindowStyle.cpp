#include <imgui.h>
#include <spatial/ui/components/styles/BorderlessWindowStyle.h>

namespace spatial::ui
{

BorderlessWindowStyle::BorderlessWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, .0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(.0F, .0F));
}

BorderlessWindowStyle::~BorderlessWindowStyle()
{
	ImGui::PopStyleVar(3);
}

} // namespace spatial::ui