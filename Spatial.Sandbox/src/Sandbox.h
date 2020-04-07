#pragma once

#include "SandboxInterface.h"

#include <spatial/render/CommonResources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/ImageBasedLight.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/RenderingSystem.h>

namespace fl = filament;

namespace spatial
{

class Sandbox
{
private:
	SandboxInterface m_interface{};

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

public:
	Sandbox(RenderingSystem& rendering);

	void onStart();

	void onUpdateFrame(float delta);
};

} // namespace spatial