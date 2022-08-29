#include <imgui.h>
#include <spatial/ui/components/CollapsingHeader.h>
#include <spatial/ui/components/Components.h>

namespace spatial::ui
{

CollapsingHeader::CollapsingHeader(const std::string_view name, bool defaultOpen)
{
	spacing(2);
	mRendered = ImGui::CollapsingHeader(name.data(), &mOpened,
										defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
}

CollapsingHeader::~CollapsingHeader()
{
}

bool CollapsingHeader::onClose() const
{
	return !mOpened;
}

bool CollapsingHeader::isOpen() const
{
	return mRendered;
}

} // namespace spatial::ui