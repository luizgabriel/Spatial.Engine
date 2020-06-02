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

namespace fl = filament;
using namespace filament::math;

namespace spatial
{

Sandbox::Sandbox(RenderingSystem& renderingSystem)
	: m_engine{renderingSystem.getEngine()},
	  m_camera{renderingSystem.getMainCamera()},
	  m_view{renderingSystem.getMainView()},

	  m_cam{{3.89263f, -0.413847}, {300.0f, 300.0f, 300.0f}},
	  m_cameraData{.5f, 500.0f},
	  m_textureManager{m_engine},
	  m_materialManager{m_engine},
	  m_materialInstanceManager{m_engine},
	  m_meshManager{m_engine},

	  m_scene{createScene(m_engine)},
	  m_light{createEntity(m_engine)},
	  m_skybox{createResource<fl::Skybox>(m_engine)},
	  m_indirectLight{createResource<fl::IndirectLight>(m_engine)}
{
	m_view->setScene(m_scene.get());

	auto& debugCubeTexture = m_textureManager.load("debug_cube", "textures/debug_cube.png");
	auto& defaultMaterial = m_materialManager.load("default", "materials/default");

	auto [skyboxPath, iblPath, shPath] = parseKtxFolder("textures/pillars_2k");

	auto& skyboxTexture = m_textureManager.load<&createKtxTexture>("skybox", skyboxPath);
	m_skybox = createSkybox(m_engine, skyboxTexture.get());

	const auto bands = parseShFile(shPath);
	auto& iblTexture = m_textureManager.load<&createKtxTexture>("skybox_ibl", iblPath);
	m_indirectLight = createImageBasedLight(m_engine, iblTexture.get(), bands);

	m_scene->setSkybox(m_skybox.get());
	m_scene->setIndirectLight(m_indirectLight.get());

	const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
	auto& defaultMaterialInstance = m_materialInstanceManager.load(1, defaultMaterial.get());
	defaultMaterialInstance->setParameter("albedo", debugCubeTexture.get(), sampler);
	defaultMaterialInstance->setParameter("clearCoat", 0.7f);
	defaultMaterialInstance->setParameter("clearCoatRoughness", 0.0f);

	m_scene->addEntity(m_light.get());

	auto& mesh = m_meshManager.load("debug_cube", "models/debug_cube", defaultMaterialInstance.get());
	m_scene->addEntity(mesh.get());
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
