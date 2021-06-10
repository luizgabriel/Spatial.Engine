#include <imgui.h>
#include <spatial/ui/components/Menu.h>

namespace spatial::ui
{

Menu::Menu(const std::string_view name)
{
	mOpened = ImGui::BeginMenu(name.data());
}

bool Menu::item(const std::string_view name)
{
	assert(isOpen());
	return ImGui::MenuItem(name.data());
}

bool Menu::item(const std::string_view name, const std::string_view shortcut)
{
	assert(isOpen());
	return ImGui::MenuItem(name.data(), shortcut.data());
}

Menu::~Menu()
{
	if (mOpened)
		ImGui::EndMenu();
}

bool Menu::isOpen()
{
	return mOpened;
}

} // namespace spatial::ui