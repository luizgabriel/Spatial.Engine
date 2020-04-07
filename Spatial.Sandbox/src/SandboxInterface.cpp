#include "SandboxInterface.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/common/Key.h>
#include <spatial/input/Input.h>

using namespace filament::math;

namespace ImGui
{

void SpatialDockLayout(ImGuiID dockspaceId)
{
	DockBuilderRemoveNode(dockspaceId); // Clear out existing layout
	DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);

	auto dockId = dockspaceId;
	const auto dockRight = DockBuilderSplitNode(dockId, ImGuiDir_Right, 0.20f, nullptr, &dockId);
	// auto dockBottom = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Down, 0.20f, NULL, &dockId);

	DockBuilderDockWindow("Properties", dockRight);
	DockBuilderFinish(dockspaceId);
}

void BeginSpatialEngine(bool* openedPropertiesPtr)
{
	static auto dockFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	static auto windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
	                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	                          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
	                          ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = GetMainViewport();

	SetNextWindowPos(viewport->Pos);
	SetNextWindowSize(viewport->Size);
	SetNextWindowViewport(viewport->ID);
	PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	Begin("DockSpace", nullptr, windowFlags);
	PopStyleVar();

	PopStyleVar(2);

	// DockSpace
	const ImGuiID dockspaceId = GetID("SpatialDockSpace");

	if (DockBuilderGetNode(dockspaceId) == nullptr)
	{
		SpatialDockLayout(dockspaceId);
	}

	DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockFlags);

	if (BeginMainMenuBar())
	{
		Text("Spatial Engine");
		Separator();

		if (BeginMenu("Options"))
		{
			MenuItem("Properties", NULL, openedPropertiesPtr);
			// ImGui::MenuItem("Console", NULL, &gOpenedLogging);
			EndMenu();
		}

		EndMainMenuBar();
	}
}

} // namespace ImGui

namespace spatial
{

SandboxInterface::SandboxInterface()
	: m_showEngineGui{true},
	  m_openedPropertiesWindow{true}
{
}

void SandboxInterface::render()
{
	if (Input::released(Key::G))
		m_showEngineGui = !m_showEngineGui;

	if (m_showEngineGui)
	{
		ImGui::BeginSpatialEngine(&m_openedPropertiesWindow);

		if (m_openedPropertiesWindow)
		{
			if (ImGui::Begin("Properties", &m_openedPropertiesWindow))
			{
				//ImGui::Text("View Controls");
				//ImGui::ColorPicker4("Clear color", &viewData.clearColor[0]);

				ImGui::Separator();
				ImGui::Text("Camera Controls");
				ImGui::InputFloat3("Camera center", &cameraData.center[0]);
				ImGui::SliderFloat("X-Distance", &cameraData.eye[0], -5000.0f, 5000.0f);
				ImGui::SliderFloat("Y-Distance", &cameraData.eye[1], -5000.0f, 5000.0f);
				ImGui::SliderFloat("Z-Distance", &cameraData.eye[2], -5000.0f, 5000.0f);

				ImGui::Separator();
				ImGui::Text("Material Properties");
				ImGui::SliderFloat("Metallic", &materialData.metallic, .0f, 1.0f);
				ImGui::SliderFloat("Roughness", &materialData.roughness, .0f, 1.0f);
				ImGui::SliderFloat("Clearcoat", &materialData.clearCoat, .0f, 1.0f);
				ImGui::SliderFloat("Clearcoat Roughness", &materialData.clearCoatRoughness, .0f, 1.0f);
			}
			ImGui::End();
		}

		ImGui::End();
	}
}

} // namespace spatial