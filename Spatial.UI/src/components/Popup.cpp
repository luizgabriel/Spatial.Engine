#include <imgui.h>
#include <spatial/ui/components/Popup.h>

namespace spatial::ui
{

Popup::Popup(const std::string_view name, bool useContext)
{
	mOpen = useContext ? ImGui::BeginPopupContextWindow(name.data()) : ImGui::BeginPopup(name.data());
}

bool Popup::isOpen() const
{
	return mOpen;
}

Popup::~Popup()
{
	if (mOpen) ImGui::EndPopup();
}


}