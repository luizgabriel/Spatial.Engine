#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/ImageBasedLight.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/RenderingSystem.h>

namespace fl = filament;

namespace spatial
{
struct CameraData
{
	filament::math::float3 front{};
	filament::math::float3 pos{};
	filament::math::float3 up{};

	float speed{1.0f};
	float yaw{.0f};
	float pitch{.0f};
	float sensitivity{0.1f};
};

struct MaterialData
{
	float metallic;
	float roughness;
	float clearCoat;
	float clearCoatRoughness;
};

class Sandbox
{
private:
	fl::Engine* m_engine;
	fl::Camera* m_camera;
	fl::View* m_view;

	Scene m_scene;
	Material m_material;
	MaterialInstance m_instance;
	Texture m_texture;
	Entity m_light;
	Mesh m_sphereMesh;
	ImageBasedLight m_ibl;

	CameraData cameraData{
		{301, 201, 301},
		{300, 200, 300},
		{0, 1.0f, 0},
		750.0f,
	};

	MaterialData materialData{
		0.5f,
		0.4f,
		0.7f,
		0.0f,
	};

	filament::math::float2 lastMouse{.0f, .0f};

	bool showEngineGui{true};
	bool enabledCameraController{true};

public:
	Sandbox(RenderingSystem& rendering);

	void attach(EventQueue& queue);

	void detach(EventQueue& queue);

	void onEvent(const MouseMovedEvent& e);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();
};
} // namespace spatial
