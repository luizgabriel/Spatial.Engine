#include <imgui.h>
#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>

namespace spatial::ui
{

Collapse::Collapse(const std::string_view name, bool defaultOpen)
{
	spacing(2);
	mRendered = ImGui::CollapsingHeader(name.data(), &mOpened,
									   defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);

	if (mRendered) {
		ImGui::Indent();
		ImGui::PushID(name.data());
	}
}

Collapse::~Collapse()
{
	if (mRendered) {
		ImGui::PopID();
		ImGui::Unindent();
	}
}

bool Collapse::onClose() const
{
	return !mOpened;
}

bool Collapse::isOpen() const
{
	return mRendered;
}

} // namespace spatial::ui