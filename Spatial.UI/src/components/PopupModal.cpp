#include <spatial/ui/components/PopupModal.h>
#include <imgui.h>

namespace spatial::ui
{

PopupModal::PopupModal(const std::string_view name)
{
	mOpened = ImGui::BeginPopupModal(name.data(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
}

PopupModal::~PopupModal()
{
	if (mOpened)
		ImGui::EndPopup();
}

bool PopupModal::isOpen()
{
	return mOpened;
}

void PopupModal::close()
{
	ImGui::CloseCurrentPopup();
}

}