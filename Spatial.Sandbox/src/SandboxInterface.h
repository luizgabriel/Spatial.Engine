#pragma once

#include <spatial/spatial.h>

namespace ImGui
{

void SpatialDockLayout(ImGuiID dockspaceId);

bool BeginSpatialEngine(bool *openedPropertiesPtr);

} // namespace ImGui

namespace spatial::sandbox
{

class SandboxInterface
{
    struct CameraData
    {
        math::float3 eye;
        math::float3 center;
    };

    struct ViewData {
        math::float4 clearColor;
    };

    struct MaterialData {
        float metallic;
        float roughness;
        float clearCoat;
        float clearCoatRoughness;
    };

private:
    bool m_showEngineGui;
    bool m_openedPropertiesWindow;

public:
    CameraData cameraData;
    ViewData viewData;
    MaterialData materialData;

    SandboxInterface(core::Application& app);

    void onStart();
    void onUpdateFrame(float delta);
};

}