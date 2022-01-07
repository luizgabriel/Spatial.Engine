#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Search.h>
#include <imgui.h>

namespace spatial::ui
{

bool Search::searchText(std::string& search)
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.80f);
	return ui::inputText("Search", search);
}

} // namespace spatial::ui