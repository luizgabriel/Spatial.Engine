#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Resources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/ImageBasedLight.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/MouseMovementCameraController.h>

namespace fl = filament;

namespace spatial
{

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
	MouseMovementCameraController m_cam;

	bool showEngineGui{true};
	bool enabledCameraController{true};

public:
	Sandbox(filament::Engine* engine, filament::Camera* mainCamera, filament::View* mainView);

	void attach(EventQueue& queue);

	void detach(EventQueue& queue);

	void onEvent(const MouseMovedEvent& e);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();
};
} // namespace spatial
