#include "Sandbox.h"

#include <spatial/render/CommonResources.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/input/Input.h>
#include <spatial/core/Logger.h>

#include <imgui.h>

#include <filament/RenderableManager.h>
#include <filament/LightManager.h>
#include <filament/TextureSampler.h>

#include <cmath>


namespace fl = filament;
using namespace filament::math;

template <typename AngleType>
auto radians(AngleType angle) noexcept
{
	static const AngleType alpha = std::acos(-1) / AngleType{180};
	return angle * alpha;
}

namespace spatial
{

Logger gLogger = createDefaultLogger();

Sandbox::Sandbox(RenderingSystem& rendering)
	: m_engine{rendering.getEngine()},
	  m_camera{rendering.getMainCamera()},
	  m_view{rendering.getMainView()},

	  m_scene{createScene(m_engine)},
	  m_material{createMaterial(m_engine, "materials/default")},
	  m_instance{m_engine, m_material->createInstance()},
	  m_texture{createTexture(m_engine, "textures/debug_cube.png")},
	  m_light{createEntity(m_engine)},
	  m_sphereMesh{createMesh(m_engine, m_instance.get(), "models/debug_cube")},
	  m_ibl{createIblFromKtx(m_engine, "textures/pillars_2k")}
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
	// m_instance->setParameter("baseColor", fl::RgbType::sRGB, {0.8, 0.0, 0.0});

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
	const auto mousePos = Input::mouse();
	const auto offset = (lastMouse - mousePos) * cameraData.sensitivity;

	if (enabledCameraController)
	{
		cameraData.yaw += offset.x;
		cameraData.pitch += offset.y;

		cameraData.pitch = std::clamp(cameraData.pitch, -89.0f, 89.0f);
	}

	const float3 direction{cos(radians(cameraData.yaw)) * cos(radians(cameraData.pitch)), sin(radians(cameraData.pitch)),
						   sin(radians(cameraData.yaw)) * cos(radians(cameraData.pitch))};

	cameraData.front = normalize(direction);

	lastMouse = mousePos;
}

void Sandbox::onUpdateFrame(float delta)
{
	if (Input::released(Key::C))
		enabledCameraController = !enabledCameraController;
	
	if (enabledCameraController)
	{
		const float cameraSpeed = cameraData.speed * delta;

		if (Input::pressed(Key::W))
			cameraData.pos += cameraSpeed * cameraData.front;

		if (Input::pressed(Key::S))
			cameraData.pos -= cameraSpeed * cameraData.front;

		if (Input::pressed(Key::A))
			cameraData.pos -= normalize(cross(cameraData.front, cameraData.up)) * cameraSpeed;

		if (Input::pressed(Key::D))
			cameraData.pos += normalize(cross(cameraData.front, cameraData.up)) * cameraSpeed;
	}

	m_instance->setParameter("metallic", materialData.metallic);
	m_instance->setParameter("roughness", materialData.roughness);
	m_instance->setParameter("clearCoat", materialData.clearCoat);
	m_instance->setParameter("clearCoatRoughness", materialData.clearCoatRoughness);

	m_camera->lookAt(cameraData.pos, cameraData.pos + cameraData.front, cameraData.up);
}

void Sandbox::onDrawGui()
{
	if (Input::released(Key::G))
		showEngineGui = !showEngineGui;

	if (showEngineGui)
	{
		ImGui::SetNextWindowSize({.0f, .0f}, ImGuiCond_Always);

		ImGui::Begin("Camera");
		ImGui::Checkbox("Enabled (Press C to toggle)", &enabledCameraController);
		ImGui::InputFloat3("Position", &cameraData.pos[0]);
		ImGui::InputFloat3("Front", &cameraData.front[0]);
		ImGui::InputFloat3("Up", &cameraData.up[0]);
		ImGui::InputFloat("Yaw", &cameraData.yaw);
		ImGui::InputFloat("Pitch", &cameraData.pitch);
		ImGui::DragFloat("Velocity", &cameraData.speed, 5.0f, 100.0f, 1000.0f);
		ImGui::DragFloat("Sensitivity", &cameraData.sensitivity, .001f, .001f, 5.0f);
		ImGui::End();
	}
}
} // namespace spatial::sandbox
