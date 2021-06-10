#include <imgui.h>
#include <spatial/ui/components/ImGuiListPanel.h>

namespace spatial::ui
{

ImGuiListPanel::ImGuiListPanel(const std::string_view name) : mWindow{name}
{
}

bool ImGuiListPanel::addItem(const std::string_view name, bool active)
{
	ImGuiTreeNodeFlags flags = active ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
	flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

	bool opened = ImGui::TreeNodeEx("##", flags, "%s", name.data());
	bool clicked = ImGui::IsItemClicked();

	if (opened)
		ImGui::TreePop();

	return clicked;
}

bool ImGuiListPanel::selectedNone()
{
	return ImGui::IsMouseDown(0) && ImGui::IsWindowHovered();
}

} // namespace spatial::ui