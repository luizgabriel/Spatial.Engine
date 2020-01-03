#include "utils.h"

namespace sp = spatial::render;
namespace fs = std::filesystem;
namespace fl = filament;

class SandboxLayer
{
private:
    fs::path m_assets;

    sp::Scene m_scene;

    sp::Material m_material;
    sp::MaterialInstance m_instance;

    sp::Sphere m_sphere;
    sp::Entity m_light;

public:
    SandboxLayer()
        : m_assets{fs::path{"assets"} / fs::path{"sandbox"}},
          m_scene{engine().createScene()},
          m_material{engine().createMaterial( read(m_assets / "materials" / "plastic.filamat") )},
          m_instance{engine().createInstance(m_material)},
          m_sphere{engine(), m_material, true},
          m_light{}
    {
        app().connect(this);
    }

    ~SandboxLayer()
    {
        app().disconnect(this);
    }

    void onStart()
    {
        auto [w, h] = window().getFrameBufferSize();

        camera()->setExposure(16.0f, 1 / 125.0f, 100.0f);
        camera()->setProjection(45.0, double(w) / h, 0.1, 50, fl::Camera::Fov::VERTICAL);

        fl::math::float3 eye{0, 0, 4}, center{0, 0, 0}, up{0, 1, 0};
        camera()->lookAt(eye, center, up);

        view()->setScene(m_scene.get());
        view()->setViewport({0, 0, w, h});
        view()->setClearTargets(true, true, true);
        view()->setClearColor({.0f, .0f, .0f, 1.0f});

        m_instance->setParameter("baseColor", fl::RgbType::sRGB, {0.8, 0.0, 0.0});
        m_instance->setParameter("roughness", 0.5f);
        m_instance->setParameter("clearCoat", 1.0f);
        m_instance->setParameter("clearCoatRoughness", 0.3f);

        m_sphere.setPosition(eye);
        m_sphere.setRadius(1.0f);

        fl::LightManager::Builder(fl::LightManager::Type::SUN)
            .color(fl::Color::toLinear<fl::ACCURATE>({0.98f, 0.92f, 0.89f}))
            .intensity(110000.0f)
            .direction({0.6f, -1.0f, -0.8f})
            .castShadows(true)
            .sunAngularRadius(1.9f)
            .sunHaloSize(10.0f)
            .sunHaloFalloff(80.0f)
            .build(engine().get_ref(), m_light.get());

        m_scene->addEntity(m_sphere.getEntity().get());
        m_scene->addEntity(m_light.get());
    }

    void onUpdate(float delta)
    {
        //
    }

    void onFinish()
    {
        //
    }
};

int main(int arc, char *argv[])
{
    fs::path executable{argv[0]};

    basePath() = executable.parent_path();
    SandboxLayer gameLayer;

    return app().run();
}