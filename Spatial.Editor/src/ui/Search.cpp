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

void Search::componentFilter(spatial::ui::ComponentFilter& componentFilter)
{
	auto availableWidth = ImGui::GetContentRegionAvail().x;
	auto activeName = magic_enum::enum_name(componentFilter);
	auto activeButtonPadding = 20.0f;
	auto dropdownWidth = 30.0f;

	auto& colors = ImGui::GetStyle().Colors;

	ImGui::Columns(3, "ComponentFilter", true);
	auto activeButtonWidth = ImGui::CalcTextSize(activeName.data()).x;
	ImGui::SetColumnWidth(0, activeButtonWidth + activeButtonPadding);
	ImGui::SetColumnWidth(1, std::max(availableWidth - activeButtonWidth - activeButtonPadding - dropdownWidth, 10.0f));
	ImGui::SetColumnWidth(2, dropdownWidth);

	ImGui::PushStyleColor(ImGuiCol_Button, colors[ImGuiCol_ButtonHovered]);
	if (ImGui::Button(activeName.data()))
		componentFilter = ComponentFilter::All;
	ImGui::PopStyleColor();

	ImGui::NextColumn();

	auto availableSpace = availableWidth - activeButtonWidth - activeButtonPadding;

	auto filters = magic_enum::enum_entries<spatial::ui::ComponentFilter>();
	size_t lastIndex = 1;
	for (auto& filter : filters)
	{
		auto name = filter.second;
		auto buttonWidth = ImGui::CalcTextSize(name.data()).x + activeButtonPadding;

		if (filter.first == componentFilter || (availableSpace - buttonWidth) <= dropdownWidth)
			continue;

		if (ImGui::Button(name.data()))
			componentFilter = filter.first;

		availableSpace -= buttonWidth;
		lastIndex++;

		ImGui::SameLine();
	}
	ImGui::NewLine();

	ImGui::NextColumn();

	if (ImGui::ArrowButton("##DropdownComponentFilter", ImGuiDir_Down)) {
		ImGui::OpenPopup("##DropdownComponentFilter");
	}

	if (ImGui::BeginPopup("##DropdownComponentFilter")) {
		// Draw the remaining filters
		for (size_t i = lastIndex; i < filters.size(); i++) {
			auto& filter = filters[i];
			auto name = filter.second;
			if (filter.first == componentFilter)
				continue;

			if (ImGui::Selectable(name.data())) {
				componentFilter = filter.first;
			}
		}
		ImGui::EndPopup();
	}

	ImGui::Columns(1);
}

} // namespace spatial::ui