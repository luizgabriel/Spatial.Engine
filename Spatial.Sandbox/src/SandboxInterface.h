#pragma once

#include <spatial/spatial.h>

namespace ImGui
{

void SpatialDockLayout(ImGuiID dockspaceId);

bool BeginSpatialEngine(bool *openedPropertiesPtr);

} // namespace ImGui

namespace spatial::sandbox
{

class Sandbox;

class SandboxInterface
{
private:
    filament::View* m_view;
    filament::Camera* m_camera;

    math::float3 m_cameraEye;
    math::float3 m_cameraCenter;
    math::float4 m_clearColor;
    math::float3 m_spherePosition;
    float m_sphereScale;
    float m_sphereRotation;

    bool m_showEngineGui;
    bool m_openedPropertiesWindow;

public:
    SandboxInterface(core::Application& app);

    void onStart();
    void onUpdateFrame(float delta);

    friend class Sandbox;
};

}