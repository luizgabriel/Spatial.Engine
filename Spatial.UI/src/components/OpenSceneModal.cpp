#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/OpenSceneModal.h>

namespace spatial::ui
{

OpenSceneModal::OpenSceneModal(std::string& openPath) : mModal{"Open Scene"}
{
	if (mModal.isOpen())
	{
		ui::inputText("Scene File Path", openPath);

		ImGui::Separator();

		mIsConfirmed = ImGui::Button("Open");
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

bool OpenSceneModal::onConfirm()
{
	return mModal.isOpen() && mIsConfirmed;
}

}