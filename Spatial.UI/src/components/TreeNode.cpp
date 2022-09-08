#include "spatial/ui/components/TreeNode.h"
#include "spatial/ui/components/Basic.h"
#include <imgui.h>

namespace spatial::ui
{

TreeNode::TreeNode(const std::string_view name, bool defaultOpen)
{
	spacing(2);
	mOpened =
		ImGui::TreeNodeEx(name.data(), ImGuiTreeNodeFlags_SpanFullWidth
										   | (defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None));
}

TreeNode::~TreeNode()
{
	if (mOpened)
		ImGui::TreePop();
}

bool TreeNode::isOpen() const
{
	return mOpened;
}

} // namespace spatial::ui