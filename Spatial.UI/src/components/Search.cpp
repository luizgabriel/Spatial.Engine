#include <spatial/ui/components/Search.h>
#include <spatial/ui/components/Components.h>

namespace spatial::ui
{

bool Search::searchText(std::string& search)
{
	return ui::inputText("Search: ", search);
}

} // namespace spatial::ui