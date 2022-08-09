#include <imgui.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/Search.h>

namespace spatial::ui
{

bool Search::text(std::string& search, const filament::Texture* texture)
{
	auto color = ImVec4(0.27f, 0.27f, 0.27f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	ui::imageButton(texture, math::vec2{15.0f}, Icons::search.uv());
	ImGui::PopStyleColor(3);

	ImGui::SameLine(.0f, .0f);
	ui::spanToAvailWidth();
	return ui::inputText("##Search", search);
}

} // namespace spatial::ui