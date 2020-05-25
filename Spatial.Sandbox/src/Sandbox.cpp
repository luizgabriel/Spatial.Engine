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

Logger gLogger = createDefaultLogger();

Sandbox::Sandbox(RenderingSystem renderingSystem)
	: m_engine{renderingSystem.getEngine()},
	  m_camera{renderingSystem.getMainCamera()},
	  m_view{renderingSystem.getMainView()},

	  m_scene{createScene(m_engine)},
	  m_material{createMaterial(m_engine, "materials/default")},
	  m_instance{createMaterialInstance(m_engine, m_material.get())},
	  m_texture{createTexture(m_engine, "textures/debug_cube.png")},
	  m_light{createEntity(m_engine)},
	  m_sphereMesh{createMesh(m_engine, m_instance.get(), "models/debug_cube")},
	  m_ibl{createIblFromKtx(m_engine, "textures/railway_bridge_2k")},
	  m_cam{10.0f, 220.0f, -26.0f}
{
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
	m_view->setClearColor({0, 0, 0, 0});

	const auto sampler = fl::TextureSampler{fl::TextureSampler::MinFilter::LINEAR, fl::TextureSampler::MagFilter::LINEAR};
	m_instance->setParameter("albedo", m_texture.get(), sampler);
	m_instance->setParameter("clearCoat", 0.7f);
	m_instance->setParameter("clearCoatRoughness", 0.0f);

	m_scene->setSkybox(m_ibl.getSkybox());
	m_scene->setIndirectLight(m_ibl.getLight());
	m_scene->addEntity(m_light.get());
	m_scene->addEntity(m_sphereMesh.get());

	auto& rcm = m_engine->getRenderableManager();
	const auto ri = rcm.getInstance(m_sphereMesh.get());
	rcm.setCastShadows(ri, false);
}

void Sandbox::onEvent(const MouseMovedEvent& e)
{
	const auto pos = Mouse::position();
	if (enabledCameraController)
	{
		m_cam.onMouseMoved(pos);
		Mouse::move({.5f, .5f});
		gLogger.info("Yaw: {}, Pitch: {}", m_cam.getYaw(), m_cam.getPitch());
	}
}

void Sandbox::onUpdateFrame(float delta)
{
	if (Keyboard::released(Key::C))
		enabledCameraController = !enabledCameraController;

	if (Keyboard::released(Key::G))
		showEngineGui = !showEngineGui;

	m_instance->setParameter("metallic", m_materialData.metallic);
	m_instance->setParameter("roughness", m_materialData.roughness);

	constexpr auto cameraPos = float3{300.0f, 300.0f, 300.0f};
	constexpr auto cameraUp = float3{.0f, 1.0f, .0f};
	m_camera->lookAt(cameraPos, cameraPos + m_cam.getDirection(), cameraUp);
}

void Sandbox::onDrawGui()
{
	if (!showEngineGui)
		return;

	ImGui::Begin("Material");

	ImGui::DragFloat("Metallic", &m_materialData.metallic, .001f, .0f, 1.0f);
	ImGui::DragFloat("Roughness", &m_materialData.roughness, .001f, .0f, 1.0f);

	ImGui::End();
}

} // namespace spatial
