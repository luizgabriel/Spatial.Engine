#include <imgui.h>
#include <spatial/ui/components/NewSceneModal.h>

namespace spatial::ui
{

ui::NewSceneModal::NewSceneModal() : mModal{"New Scene"}
{
	if (mModal.isOpen())
	{
		ImGui::Text("Are you sure? If you continue, all unsaved changes will be discarded.");

		ImGui::Separator();

		mIsConfirmed = ImGui::Button("Discard unsaved changes and create a new scene.");
		if (mIsConfirmed)
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
			mModal.close();
	}
}

bool ui::NewSceneModal::onConfirm()
{
	return mModal.isOpen() && mIsConfirmed;
}

}