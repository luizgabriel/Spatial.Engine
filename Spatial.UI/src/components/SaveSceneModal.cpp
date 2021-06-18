#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/SaveSceneModal.h>

namespace spatial::ui
{

SaveSceneModal::SaveSceneModal(std::string& savePath) : mModal{"Save Scene"}
{
	if (mModal.isOpen())
	{
		ui::inputText("Scene File Path", savePath);

		ImGui::Separator();

		mConfirmed = ImGui::Button("Save");
		if (mConfirmed)
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
			mModal.close();
	}
}

bool SaveSceneModal::onConfirm()
{
	return mModal.isOpen() && mConfirmed;
}

}