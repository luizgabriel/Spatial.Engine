#include "Sandbox.h"

#include <spatial/render/Resources.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/input/Keyboard.h>
#include <spatial/core/Logger.h>

#include <imgui.h>

#include <filament/RenderableManager.h>
#include <filament/LightManager.h>
#include <filament/TextureSampler.h>

#include <cmath>
#include <spatial/input/Mouse.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/RegistryUtils.h>
#include <spatial/ecs/Components.h>

#include "Components.h"

namespace fl = filament;
using namespace filament::math;

namespace MM
{
template <>
void ComponentEditorWidget<spatial::ecs::DebugCube>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<spatial::ecs::DebugCube>(e);
	ImGui::DragFloat("Metallic", &t.metallic, 0.01f, .0f, 1.0f);
	ImGui::DragFloat("Roughness", &t.roughness, 0.01f, .0f, 1.0f);
	ImGui::DragFloat("Clear Coat", &t.clearCoat, 0.01f, .0f, 1.0f);
	ImGui::DragFloat("Clear Coat Roughness", &t.clearCoatRoughness, 0.01f, .0f, 1.0f);
}

template <>
void ComponentEditorWidget<spatial::ecs::Transform>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<spatial::ecs::Transform>(e);
	ImGui::DragFloat3("Position", &t.position[0], 1.0f);
	ImGui::DragFloat3("Scale", &t.scale[0], .1f);
}


} // namespace MM

namespace spatial
{

Sandbox::Sandbox(RenderingSystem& renderingSystem)
	: m_engine{renderingSystem.getEngine()},
	  m_camera{renderingSystem.getMainCamera()},
	  m_view{renderingSystem.getMainView()},

	  m_cam{{3.89263f, -0.413847}, {300.0f, 300.0f, 300.0f}},
	  m_cameraData{.5f, 500.0f},

	  m_scene{createScene(m_engine)},
	  m_skyboxTexture{createKtxTexture(m_engine, "textures/pillars_2k/pillars_2k_skybox.ktx")},
	  m_iblTexture{createKtxTexture(m_engine, "textures/pillars_2k/pillars_2k_ibl.ktx")},
	  m_indirectLight{createImageBasedLight(m_engine, m_iblTexture.get(), "textures/pillars_2k/sh.txt")},
	  m_skybox{createSkybox(m_engine, m_skyboxTexture.get())},

	  m_registry{},
	  m_renderableSystem{m_scene.get()},
	  m_debugCubeSystem{m_engine},
	  m_transformSystem{m_engine},
	  m_editor{}
{
	m_editor.registerComponent<ecs::DebugCube>("Debug Cube");
	m_editor.registerComponent<ecs::Transform>("Transform");
	connect<ecs::DebugCube>(m_registry, m_debugCubeSystem);
	connect<ecs::Renderable>(m_registry, m_renderableSystem);

	m_view->setScene(m_scene.get());
	m_scene->setIndirectLight(m_indirectLight.get());
	m_scene->setSkybox(m_skybox.get());

	auto entity = m_registry.create();
	m_registry.emplace<ecs::Transform>(entity);
	m_registry.emplace<ecs::DebugCube>(entity, 0.5f, 0.5f, 0.5f, 0.5f);
}

void Sandbox::onEvent(const MouseMovedEvent& e)
{
	if (enabledCameraController)
	{
		m_cam.onMouseMoved(Mouse::position(), m_cameraData.sensitivity);
		Mouse::move({.5f, .5f});
	}
}

float3 defaultInputAxis()
{
	return {
		Keyboard::axis(Key::W, Key::S),
		Keyboard::axis(Key::D, Key::A),
		Keyboard::axis(Key::Space, Key::LShift),
	};
}

void Sandbox::onUpdateFrame(float delta)
{
	if (Keyboard::released(Key::MouseLeft))
		enabledCameraController = false;

	if (Keyboard::released(Key::MouseRight))
		enabledCameraController = true;

	if (Keyboard::released(Key::G))
		showEngineGui = !showEngineGui;

	m_debugCubeSystem.onUpdate(m_registry);
	m_transformSystem.onUpdate(m_registry);
	m_cam.onUpdate(m_camera, delta * m_cameraData.velocity * defaultInputAxis());

	m_registry.each([this](auto entity) { m_editor.render(m_registry, entity); });
}

void Sandbox::onDrawGui()
{
	if (!showEngineGui)
		return;

	ImGui::Begin("Camera Settings");

	ImGui::Text("Use the WASD to move in the scene.");

	if (enabledCameraController)
		ImGui::Text("Left Click to turn the camera movement OFF:");
	else
		ImGui::Text("Right Click to turn the camera movement ON:");

	ImGui::Checkbox("Enabled", &enabledCameraController);

	ImGui::Separator();

	ImGui::DragFloat("Yaw", &m_cam.rotation.x, .001f);
	ImGui::DragFloat("Pitch", &m_cam.rotation.y, .001f, -halfPi<float>, halfPi<float>);

	ImGui::Separator();

	ImGui::DragFloat("Sensitivity", &m_cameraData.sensitivity, .001f, .1f, 5.0f);
	ImGui::DragFloat("Velocity", &m_cameraData.velocity, 1.0f, 100.0f, 2000.0f);

	ImGui::End();
}

} // namespace spatial
