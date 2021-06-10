#include <imgui.h>
#include <spatial/ui/components/styles/WindowPaddingStyle.h>

namespace spatial::ui
{

WindowPaddingStyle::WindowPaddingStyle() : WindowPaddingStyle{{.0f}}
{
}

WindowPaddingStyle::WindowPaddingStyle(const math::float2& padding)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding.x, padding.y));
}

WindowPaddingStyle::~WindowPaddingStyle()
{
	ImGui::PopStyleVar();
}

} // namespace spatial::ui