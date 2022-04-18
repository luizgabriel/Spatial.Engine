#include <imgui.h>
#include <spatial/ui/components/PopupModal.h>

namespace spatial::ui
{

PopupModal::PopupModal(const std::string_view name)
{
	auto& io = ImGui::GetIO();
    auto pos = ImVec2{io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f};
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	auto flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings;
	mOpened = ImGui::BeginPopupModal(name.data(), nullptr, flags);
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