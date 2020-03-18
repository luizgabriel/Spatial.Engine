#pragma once

#include "SandboxInterface.h"

#include <spatial/render/CommonResources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/ImageBasedLight.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/RenderingSystem.h>

namespace fl = filament;

namespace spatial::sandbox
{

class Sandbox
{
private:
	SandboxInterface m_interface;

	fl::Engine* m_engine;
	fl::Camera* m_camera;
	fl::View* m_view;

	render::Scene m_scene;
	render::Material m_material;
	render::MaterialInstance m_instance;
	render::Texture m_texture;
	render::Entity m_light;
	render::Mesh m_sphereMesh;
	render::ImageBasedLight m_ibl;

public:
	Sandbox(render::RenderingSystem& rendering);

	void onStart();

	void onUpdateFrame(float delta);
};

} // namespace spatial::sandbox