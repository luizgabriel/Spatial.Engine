#include <imgui.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/styles/BorderlessWindowStyle.h>

namespace spatial::ui
{

DockSpace::DockSpace(const std::string_view name)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	{
		auto style = BorderlessWindowStyle{};
		ImGui::Begin(name.data(), nullptr,
					 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
						 | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
						 | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
	}

	// DockSpace
	static ImGuiID dockSpaceId = ImGui::GetID(name.data());
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

DockSpace::~DockSpace()
{
	ImGui::End(); // DockSpace
}

}