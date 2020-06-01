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

namespace fl = filament;
using namespace filament::math;

namespace spatial
{

Sandbox::Sandbox(RenderingSystem& renderingSystem)
	: m_engine{renderingSystem.getEngine()},
	  m_camera{renderingSystem.getMainCamera()},
	  m_view{renderingSystem.getMainView()},
	  m_scene{createScene(m_engine)},
	  m_light{createEntity(m_engine)},
	  m_ibl{createIblFromKtx(m_engine, "textures/pillars_2k")},
	  m_cam{{3.89263f, -0.413847}, {300.0f, 300.0f, 300.0f}},
	  m_cameraData{.5f, 500.0f},
	  m_textureManager{m_engine},
	  m_materialManager{m_engine},
	  m_materialInstanceManager{m_engine},
	  m_meshManager{m_engine}
{
	m_textureManager.load("debug_cube", "textures/debug_cube.png");
	m_materialManager.load("default", "materials/default");
	m_materialInstanceManager.load(1, m_materialManager.get("default"));
	m_meshManager.load("debug_cube", "models/debug_cube", m_materialInstanceManager.get(1));
}

void Sandbox::attach(EventQueue& queue)
{
	queue.connect<MouseMovedEvent>(this);
}

void Sandbox::detach(EventQueue& queue)
{
	queue.disconnect<MouseMovedEvent>(this);
}

void Sandbox::onStart()
{
	m_view->setScene(m_scene.get());

	const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
	auto instance = m_materialInstanceManager.get(1);
	instance->setParameter("albedo", m_textureManager.get("debug_cube"), sampler);
	instance->setParameter("clearCoat", 0.7f);
	instance->setParameter("clearCoatRoughness", 0.0f);

	m_scene->setSkybox(m_ibl.getSkybox());
	m_scene->setIndirectLight(m_ibl.getLight());
	m_scene->addEntity(m_light.get());

	auto mesh = m_meshManager.get("debug_cube");
	m_scene->addEntity(mesh);

	auto& rcm = m_engine->getRenderableManager();
	const auto ri = rcm.getInstance(mesh);
	rcm.setCastShadows(ri, false);

	m_cam.onUpdate(m_camera, .0f);
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
	if (Keyboard::released(Key::C))
		enabledCameraController = !enabledCameraController;

	if (Keyboard::released(Key::G))
		showEngineGui = !showEngineGui;

	auto instance = m_materialInstanceManager.get(1);
	instance->setParameter("metallic", m_materialData.metallic);
	instance->setParameter("roughness", m_materialData.roughness);

	m_cam.onUpdate(m_camera, delta * m_cameraData.velocity * defaultInputAxis());
}

void Sandbox::onDrawGui()
{
	if (!showEngineGui)
		return;

	ImGui::Begin("Camera Settings");

	ImGui::Text("Use the WASD to move in the scene.");

	if (enabledCameraController)
		ImGui::Text("Press C turn the camera movement OFF:");
	else
		ImGui::Text("Press C turn the camera movement ON:");

	ImGui::Checkbox("Enabled", &enabledCameraController);

	ImGui::Separator();

	ImGui::DragFloat("Yaw", &m_cam.rotation.x, .001f);
	ImGui::DragFloat("Pitch", &m_cam.rotation.y, .001f, -halfPi<float>, halfPi<float>);

	ImGui::Separator();

	ImGui::DragFloat("Sensitivity", &m_cameraData.sensitivity, .001f, .1f, 5.0f);
	ImGui::DragFloat("Velocity", &m_cameraData.velocity, 1.0f, 100.0f, 2000.0f);

	ImGui::End();


	ImGui::Begin("Material Settings");

	ImGui::DragFloat("Metallic", &m_materialData.metallic, .001f, .0f, 1.0f);
	ImGui::DragFloat("Roughness", &m_materialData.roughness, .001f, .0f, 1.0f);

	ImGui::End();
}

} // namespace spatial
