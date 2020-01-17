#include "Sandbox.h"

using namespace spatial::core;
using namespace spatial::render;
using namespace spatial::math;
namespace fl = filament;

namespace spatial::sandbox
{

Sandbox::Sandbox(Application &app, RenderingSystem &rendering, SandboxInterface &interface)
    : m_interface{&interface},

      m_engine{rendering.getEngine()},
      m_transformManager{&m_engine->getTransformManager()},
      m_camera{rendering.getMainCamera()},
      m_view{rendering.getMainView()},

      m_scene{createScene(m_engine)},
      m_material{createMaterial(m_engine, "materials/plastic.filamat")},
      m_instance{m_engine, m_material->createInstance()},
      m_light{createEntity(m_engine)},
      m_sphereMesh{createMesh(m_engine, m_instance.get(), "models/sphere.filamesh")}
{
    connect(app, this);
}

void Sandbox::onStart()
{
    m_view->setScene(m_scene.get());

    m_instance->setParameter("baseColor", fl::RgbType::sRGB, {0.8, 0.0, 0.0});
    m_instance->setParameter("roughness", 0.5f);
    m_instance->setParameter("clearCoat", 1.0f);
    m_instance->setParameter("clearCoatRoughness", 0.3f);

    fl::LightManager::Builder(fl::LightManager::Type::SUN)
        .color(fl::Color::toLinear<fl::ACCURATE>({0.98f, 0.92f, 0.89f}))
        .intensity(110000.0f)
        .direction({0.6f, -1.0f, -0.8f})
        .castShadows(true)
        .sunAngularRadius(1.9f)
        .sunHaloSize(10.0f)
        .sunHaloFalloff(80.0f)
        .build(*m_engine, m_light.get());

    m_scene->addEntity(m_light.get());
    m_scene->addEntity(m_sphereMesh.get());
}

void Sandbox::onUpdateFrame(float delta)
{
    m_camera->lookAt(m_interface->m_cameraEye, m_interface->m_cameraCenter, {0, 1, 0});
    m_view->setClearColor(m_interface->m_clearColor);

    auto ti = m_transformManager->getInstance(m_sphereMesh.get());
    m_transformManager->setTransform(ti,
        mat4f{ mat3f(1.0f), m_interface->m_spherePosition }
        //* mat4f::rotation(m_interface->m_sphereRotation, float3{ 0, 0, 1.0f })
        * m_transformManager->getTransform(ti)
    );
}

} // namespace spatial::sandbox