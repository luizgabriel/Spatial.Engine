#pragma once

#include <spatial/spatial.h>
#include "SandboxInterface.h"

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
    Sandbox(core::Application& app, render::RenderingSystem& rendering);

    void onStart();

    void onUpdateFrame(float delta);

};

}