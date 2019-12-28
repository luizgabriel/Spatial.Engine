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

public:
    SandboxLayer()
        : m_scene{engine().createScene()},
          m_camera{engine().createCamera()},
          m_assets{fs::path{"assets"} / fs::path{"sandbox"}}
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
        auto& view = app().getRenderSys().getMainView();

        camera()->setExposure(16.0f, 1 / 125.0f, 100.0f);
        camera()->setProjection(45.0, double(w) / h, 0.1, 50, fl::Camera::Fov::VERTICAL);

        fl::math::float3 eye{0, 0, 4}, center{0, 0, 0},  up{0, 1, 0};
        camera()->lookAt(eye, center, up);
        
        view()->setScene(m_scene.get());
        view()->setViewport({0, 0, w, h});
        view()->setClearTargets(true, true, true);
        view()->setClearColor({.0f, 0x33 / 255.0f, 0x66 / 255.0f, 1.0f});
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