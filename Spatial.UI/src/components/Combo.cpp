#include <imgui.h>
#include <spatial/ui/components/Combo.h>

namespace spatial::ui
{

Combo::Combo(std::string_view name, std::string_view selectedItem)
{
	mIsOpen = ImGui::BeginCombo(name.data(), selectedItem.data());
}

Combo::~Combo()
{
	if (mIsOpen)
		ImGui::EndCombo();
}

bool Combo::isOpen() const
{
	return mIsOpen;
}

bool Combo::item(std::string_view name, bool isSelected)
{
	bool click = ImGui::Selectable(name.data(), isSelected);
	if (isSelected)
		ImGui::SetItemDefaultFocus();

	return click;
}

} // namespace spatial::ui