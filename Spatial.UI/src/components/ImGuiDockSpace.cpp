#include <spatial/ui/components/ImGuiDockSpace.h>

namespace spatial::ui
{

ImGuiDockSpace::ImGuiDockSpace() : mDockFlags{ImGuiDockNodeFlags_None}, mWindowFlags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar
																					 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
																					 | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
																					 | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground}
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, mWindowFlags);
	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	// DockSpace
	static ImGuiID dockSpaceId = ImGui::GetID("SpatialDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), mDockFlags);
}

ImGuiDockSpace::~ImGuiDockSpace()
{
	ImGui::End(); // DockSpace
}

}