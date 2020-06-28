#include "EditorSystem.h"

#include <spatial/input/Input.h>

#include <imgui.h>

#include <filament/LightManager.h>
#include <filament/TextureSampler.h>

#include <spatial/render/SkyboxResources.h>
#include <spatial/render/RegistryUtils.h>
#include <spatial/render/ResourceLoaders.h>

#include "Components.h"

namespace fl = filament;
using namespace filament::math;

namespace MM
{

template <>
void ComponentEditorWidget<spatial::ecs::Transform>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<spatial::ecs::Transform>(e);
	ImGui::DragFloat3("Position", &t.position[0], 1.0f);
	ImGui::DragFloat3("Scale", &t.scale[0], .1f);
}

template <>
void ComponentEditorWidget<spatial::ecs::DebugMesh>(entt::registry& reg, entt::entity e)
{
	auto& t = reg.get<spatial::ecs::DebugMesh>(e);
	ImGui::ColorPicker4("Color", &t.color[0]);
	ImGui::DragFloat("Metallic", &t.metallic, 0.01f, .0f, 1.0f);
	ImGui::DragFloat("Roughness", &t.roughness, 0.01f, .0f, 1.0f);
	ImGui::DragFloat("Clear Coat", &t.clearCoat, 0.01f, .0f, 1.0f);
	ImGui::DragFloat("Clear Coat Roughness", &t.clearCoatRoughness, 0.01f, .0f, 1.0f);
}

} // namespace MM

namespace spatial
{

EditorSystem::EditorSystem(RenderingSystem& renderingSystem)
	: m_engine{renderingSystem.getEngine()},
	  m_camera{renderingSystem.getMainCamera()},
	  m_view{renderingSystem.getMainView()},

	  m_cam{{3.89263f, -0.413847}, {300.0f, 300.0f, 300.0f}},
	  m_cameraData{.5f, 500.0f},

	  m_scene{createScene(m_engine)},
	  m_logoTexture{createTexture(m_engine, "embed://textures/spatial_engine_logo.png")},
	  m_skyboxTexture{createKtxTexture(m_engine, "assets://textures/pillars_2k/pillars_2k_skybox.ktx")},
	  m_iblTexture{createKtxTexture(m_engine, "assets://textures/pillars_2k/pillars_2k_ibl.ktx")},
	  m_indirectLight{createImageBasedLight(m_engine, m_iblTexture.ref(), "assets://textures/pillars_2k/sh.txt")},
	  m_skybox{createSkybox(m_engine, m_skyboxTexture.ref())},

	  m_registry{},
	  m_renderableSystem{m_engine, m_scene.ref()},
	  m_debugCubeSystem{m_engine},
	  m_transformSystem{m_engine},
	  m_editor{}
{
	ecs::connect<ecs::DebugMesh>(m_registry, m_debugCubeSystem);
	ecs::connect<ecs::Renderable>(m_registry, m_renderableSystem);

	m_editor.registerComponent<ecs::DebugMesh>("Debug Cube");
	m_editor.registerComponent<ecs::Transform>("Transform");

	m_view.setScene(m_scene.get());
	m_scene->setIndirectLight(m_indirectLight.get());
	m_scene->setSkybox(m_skybox.get());

	auto entity = m_registry.create();
	m_registry.emplace<ecs::Transform>(entity);
	m_registry.emplace<ecs::DebugMesh>(entity, 0.5f, 0.5f, 0.5f, 0.5f);
}

void EditorSystem::onEvent(const MouseMovedEvent& e)
{
	if (enabledCameraController)
	{
		m_cam.onMouseMoved(Input::mouse(), m_cameraData.sensitivity);
		Input::warpMouse({.5f, .5f});
	}
}

float3 defaultInputAxis()
{
	return {
		Input::axis(Key::W, Key::S),
		Input::axis(Key::D, Key::A),
		Input::axis(Key::Space, Key::LShift),
	};
}

void EditorSystem::onUpdateFrame(float delta)
{
	if (Input::released(Key::MouseLeft))
		enabledCameraController = false;

	if (Input::combined(Key::LControl, Key::MouseLeft))
		enabledCameraController = true;

	if (Input::released(Key::G))
		showEngineGui = !showEngineGui;

	m_debugCubeSystem.onUpdate(m_registry);
	m_transformSystem.onUpdate(m_registry);

	if (!enabledCameraController)
		delta = 0;
	m_cam.onUpdate(m_camera, delta * m_cameraData.velocity * defaultInputAxis());
}

void EditorSystem::onDrawGui()
{
	if (!showEngineGui)
		return;

	m_registry.each([this](auto entity) { m_editor.render(m_registry, entity); });

	ImGui::Begin("Camera Settings");

	if (enabledCameraController)
	{
		ImGui::Text("Use the WASD to move in the scene.");
		ImGui::Text("Click to turn OFF:");
	}
	else
	{
		ImGui::Text("Ctrl + Click to turn ON:");
	}

	if (ImGui::Checkbox("Camera Movement", &enabledCameraController))
	{
		Input::warpMouse({.5f, .5f});
	}

	ImGui::Separator();

	ImGui::DragFloat("Yaw", &m_cam.rotation.x, .001f);
	ImGui::DragFloat("Pitch", &m_cam.rotation.y, .001f, -halfPi<float>, halfPi<float>);

	ImGui::Separator();

	ImGui::DragFloat("Sensitivity", &m_cameraData.sensitivity, .001f, .1f, 5.0f);
	ImGui::DragFloat("Velocity", &m_cameraData.velocity, 1.0f, 100.0f, 2000.0f);

	ImGui::End();
}

} // namespace spatial
