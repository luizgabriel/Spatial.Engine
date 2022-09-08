#include "Modals.h"
#include <imgui.h>
#include <spatial/ui/components/Basic.h>

namespace spatial::ui
{

bool EditorModals::newScene()
{
	ImGui::Text("Are you sure?");
	ImGui::Text("If you continue, all unsaved changes will be discarded.");

	ui::separator(2);

	bool isConfirmed = ImGui::Button("Continue");
	if (isConfirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return isConfirmed;
}

bool EditorModals::openScene(std::string& openPath)
{
	ImGui::Text("Are you sure?");
	ImGui::Text("If you continue, all unsaved changes will be discarded.");

	ui::spacing(2);

	ui::inputText("Path", openPath, "scenes/*");

	ui::separator(2);

	bool confirmed = ImGui::Button("Continue");
	if (confirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return confirmed;
}

bool EditorModals::saveScene(std::string& savePath)
{
	ImGui::Text("Save your current scene to this file:");

	ui::spacing(2);

	ui::inputText("Scene File Path", savePath, "scenes/*");

	ui::separator(2);

	bool confirmed = ImGui::Button("Continue");
	if (confirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return confirmed;
}

bool EditorModals::openProject(std::string& openPath)
{
	ImGui::Text("Open you project source in the specified folder:");

	ui::spacing(2);

	ui::inputText("Path", openPath);

	ui::separator(2);

	bool confirmed = ImGui::Button("Continue");
	if (confirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return confirmed;
}

} // namespace spatial::ui