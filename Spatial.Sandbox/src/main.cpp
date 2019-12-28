#include "engine.h"

namespace sp = spatial::render;
namespace fs = std::filesystem;
namespace fl = filament;

struct Vertex {
    fl::math::float2 position;
    fl::math::float2 uv;
};

class SandboxLayer
{
private:
    sp::Scene m_scene;
    fs::path m_assets;

    sp::Material m_material;
    sp::MaterialInstance m_instance;

public:
    SandboxLayer()
        : m_scene{engine().createScene()},
          m_assets{fs::path{"assets"} / fs::path{"sandbox"}},
          m_material{engine().get()},
          m_instance{engine().get()}
    {
        app().connectOnStart(this);
        app().connectOnUpdate(this);
    }

    ~SandboxLayer()
    {
        app().disconnectOnStart(this);
        app().disconnectOnUpdate(this);
    }

    void onStart()
    {
        auto [w, h] = window().getFrameBufferSize();

        camera()->setExposure(16.0f, 1 / 125.0f, 100.0f);
        camera()->setProjection(45.0, double(w) / h, 0.1, 50, fl::Camera::Fov::VERTICAL);

        fl::math::float3 eye{0, 0, 4}, center{0, 0, 0},  up{0, 1, 0};
        camera()->lookAt(eye, center, up);
        
        view()->setScene(m_scene.get());
        view()->setViewport({0, 0, w, h});
        view()->setClearTargets(true, true, true);
        view()->setClearColor({.0f, 0x33 / 255.0f, 0x66 / 255.0f, 1.0f});

        auto data = read(m_assets / "materials" / "plastic.bin");
        m_material = engine().createMaterial(data);
        m_instance = engine().createInstance(m_material);

        fl::math::float3 red{0.8, 0.0, 0.0};
        m_instance->setParameter("baseColor", fl::RgbType::sRGB, red);
        m_instance->setParameter("roughness", 0.5f);
        m_instance->setParameter("clearCoat", 1.0f);
        m_instance->setParameter("clearCoatRoughness", 0.3f);
    }

    void onUpdate(float delta)
    {
        
    }

};

int main(int arc, char *argv[])
{
    SandboxLayer gameLayer;

    return app().run();
}