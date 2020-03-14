#include "SandboxInterface.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <spatial/common/Key.h>
#include <spatial/input/Input.h>

namespace fl = filament;
using namespace filament::math;
using namespace spatial::common;
using namespace spatial::input;

namespace ImGui
{

void SpatialDockLayout(ImGuiID dockspaceId)
{
    ImGui::DockBuilderRemoveNode(dockspaceId); // Clear out existing layout
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);

    auto dockId = dockspaceId;
    auto dockRight = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Right, 0.20f, NULL, &dockId);
    //auto dockBottom = ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Down, 0.20f, NULL, &dockId);

    ImGui::DockBuilderDockWindow("Properties", dockRight);
    ImGui::DockBuilderFinish(dockspaceId);
}

void BeginSpatialEngine(bool *openedPropertiesPtr)
{
    static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiID dockspaceId = ImGui::GetID("SpatialDockSpace");

    if (ImGui::DockBuilderGetNode(dockspaceId) == NULL)
    {
        ImGui::SpatialDockLayout(dockspaceId);
    }

    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockFlags);

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Spatial Engine");
        ImGui::Separator();

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Properties", NULL, openedPropertiesPtr);
            //ImGui::MenuItem("Console", NULL, &gOpenedLogging);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

} // namespace ImGui

namespace spatial::sandbox
{

SandboxInterface::SandboxInterface()
    : m_showEngineGui{true},
      m_openedPropertiesWindow{true},

      cameraData{
          .eye = {436.0f, 158.0f, -436.0f},
          .center = {-500.0f, 50.0f, 100.0f}},
      viewData{},
      materialData{
          .metallic = 0.5f,
          .roughness = 0.4f,
          .clearCoat = 0.7f,
          .clearCoatRoughness = 0.0f,
      }
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
                ImGui::Text("View Controls");
                ImGui::ColorPicker4("Clear color", &viewData.clearColor[0]);

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

} // namespace spatial::sandbox