#include <imgui.h>
#include <spatial/ui/components/Menu.h>

namespace spatial::ui
{

Menu::Menu(const std::string_view name)
{
	mOpened = ImGui::BeginMenu(name.data());
}

bool Menu::item(const std::string_view name) const
{
	if (!isOpen())
		return false;

	return itemButton(name.data());
}

bool Menu::item(const std::string_view name, const std::string_view shortcut) const
{
	if (!isOpen())
		return false;

	return itemButton(name.data(), shortcut.data());
}

Menu::~Menu()
{
	if (mOpened)
		ImGui::EndMenu();
}

bool Menu::isOpen() const
{
	return mOpened;
}

bool Menu::itemButton(std::string_view name)
{
	return ImGui::MenuItem(name.data());
}

bool Menu::itemButton(std::string_view name, std::string_view shortcut)
{
	return ImGui::MenuItem(name.data(), shortcut.data());
}

} // namespace spatial::ui