#include <imgui.h>
#include <spatial/ui/components/PopupModal.h>

namespace spatial::ui
{

PopupModal::PopupModal(const std::string_view name)
{
	mOpened = ImGui::BeginPopupModal(name.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}

PopupModal::~PopupModal()
{
	if (mOpened)
		ImGui::EndPopup();
}

bool PopupModal::isOpen() const
{
	return mOpened;
}

void PopupModal::close()
{
	ImGui::CloseCurrentPopup();
}

} // namespace spatial::ui