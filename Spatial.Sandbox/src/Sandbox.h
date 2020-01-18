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
    render::EntityResource m_light;
    render::MeshResource m_sphereMesh;

public:
    Sandbox(core::Application& app, render::RenderingSystem& rendering);

    void onStart();

    void onUpdateFrame(float delta);

};

}