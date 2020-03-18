#pragma once

#include <math/vec3.h>
#include <math/vec4.h>
#include <imgui.h>

namespace ImGui
{

void SpatialDockLayout(ImGuiID dockspaceId);

void BeginSpatialEngine(bool* openedPropertiesPtr);

} // namespace ImGui

namespace spatial::sandbox
{

class SandboxInterface
{
	struct CameraData
	{
		filament::math::float3 eye;
		filament::math::float3 center;
	};

	struct ViewData
	{
		filament::math::float4 clearColor;
	};

	struct MaterialData
	{
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

	SandboxInterface();

	void render();
};

} // namespace spatial::sandbox