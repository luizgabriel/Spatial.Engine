#include <spatial/ui/components/ImGuiCollapse.h>
#include <imgui.h>

namespace spatial::ui
{

ImGuiCollapse::ImGuiCollapse(const std::string_view name, bool defaultOpen)
{
	mRendered = ImGui::CollapsingHeader(name.data(), &mOpened,
									   defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);

	if (mRendered) {
		ImGui::Indent();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushID(name.data());
	}
}

ImGuiCollapse::~ImGuiCollapse()
{
	if (mRendered) {
		ImGui::PopID();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Unindent();
	}
}

bool ImGuiCollapse::onClose() const
{
	return !mOpened;
}

} // namespace spatial::ui