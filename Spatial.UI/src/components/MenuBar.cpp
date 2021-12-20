#include <imgui.h>
#include <spatial/ui/components/MenuBar.h>

namespace spatial::ui
{

MenuBar::MenuBar()
{
	mIsOpen = ImGui::BeginMainMenuBar();
}

MenuBar::~MenuBar()
{
	if (mIsOpen)
		ImGui::EndMainMenuBar();
}

} // namespace spatial::ui