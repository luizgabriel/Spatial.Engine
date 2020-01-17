#include "SandboxInterface.h"

namespace fl = filament;
using namespace filament::math;
using namespace spatial::common;
using namespace spatial::core;
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

bool BeginSpatialEngine(bool *openedPropertiesPtr)
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

    return true;
}

} // namespace ImGui

namespace spatial::sandbox
{

SandboxInterface::SandboxInterface(core::Application &app)
    : m_cameraEye{4, 0, -4},
      m_cameraCenter{0, 0, -4},
      m_clearColor{1.0f, 1.0f, 1.0f, 1.0f},
      m_spherePosition{ 0, 0, 0 },
      m_sphereScale{ 1.0f },
      m_sphereRotation{ .0f },

      m_showEngineGui{true},
      m_openedPropertiesWindow{true}
{
    connect(app, this);
}

void SandboxInterface::onStart()
{
}

void SandboxInterface::onUpdateFrame(float delta)
{
    if (Input::released(Key::G))
        m_showEngineGui = !m_showEngineGui;

    if (m_showEngineGui && ImGui::BeginSpatialEngine(&m_openedPropertiesWindow))
    {
        if (m_openedPropertiesWindow)
        {
            if (ImGui::Begin("Properties", &m_openedPropertiesWindow))
            {
                ImGui::Text("View Controls");
                ImGui::ColorPicker4("Clear color", &m_clearColor[0]);

                ImGui::Separator();
                ImGui::Text("Camera Controls");
                ImGui::InputFloat3("Camera eye", &m_cameraEye[0]);
                ImGui::InputFloat3("Camera center", &m_cameraCenter[0]);

                ImGui::Separator();
                ImGui::Text("Mesh Controls");
                ImGui::InputFloat3("Mesh position", &m_spherePosition[0]);
                ImGui::SliderFloat("Mesh scale", &m_sphereScale, 1.0f, 100.0f);
                ImGui::SliderAngle("Rotation", &m_sphereRotation);
            }
            ImGui::End();
        }

        ImGui::End();
    }
}

} // namespace spatial::sandbox