#include <imgui.h>
#include <spatial/ui/components/MenuBar.h>

namespace spatial::ui
{

MenuBar::MenuBar()
{
	ImGui::BeginMainMenuBar();
}

MenuBar::~MenuBar()
{
	ImGui::EndMainMenuBar();
}

} // namespace spatial::ui