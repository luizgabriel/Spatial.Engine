#include "Search.h"
#include "Icons.h"
#include "spatial/ui/components/Basic.h"
#include <imgui.h>

namespace spatial::ui
{

bool Search::text(std::string& search, graphics::OptionalTexture icons)
{
	auto color = ImVec4(0.27f, 0.27f, 0.27f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	ui::imageButton(std::move(icons), math::vec2{15.0f}, Icons::search.uv());
	ImGui::PopStyleColor(3);

	ImGui::SameLine(.0f, .0f);
	ui::spanToAvailWidth();
	return ui::inputText("##Search", search);
}

} // namespace spatial::ui