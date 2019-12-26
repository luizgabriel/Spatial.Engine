#include "engine.h"

namespace sp = spatial::render;

class SandboxLayer
{
private:
    float m_debug = 0;

    sp::Scene m_scene;
    sp::Camera m_camera;

public:
    SandboxLayer()
        : m_scene{engine().createScene()},
          m_camera{engine().createCamera()}
    {
        app().onStartEvent.connect<&SandboxLayer::onStart>(this);
        app().onFinishEvent.connect<&SandboxLayer::onFinish>(this);
        app().onUpdateEvent.connect<&SandboxLayer::onUpdate>(this);
    }

    ~SandboxLayer()
    {
        app().onStartEvent.disconnect<&SandboxLayer::onStart>(this);
        app().onFinishEvent.disconnect<&SandboxLayer::onFinish>(this);
        app().onUpdateEvent.disconnect<&SandboxLayer::onUpdate>(this);
    }

    void onStart()
    {
        auto [w, h] = window().getFrameBufferSize();
        auto& view = app().getRenderSys().getMainView();

        m_camera->setExposure(16.0f, 1 / 125.0f, 100.0f);
        m_camera->setProjection(45.0, double(w) / h, 0.1, 50, filament::Camera::Fov::VERTICAL);
        
        view->setCamera(m_camera.get());
        view->setScene(m_scene.get());
        view->setViewport({0, 0, w, h});
        view->setClearTargets(true, true, true);
        view->setClearColor({.0f, 0x33 / 255.0f, 0x66 / 255.0f, 1.0f});
    }

    void onFinish()
    {

    }

    void onUpdate(float delta)
    {
        if (m_debug > 2.0f)
        {
            std::cout << fmt::format("FPS: {}\n", 1 / delta);
            m_debug = 0;
        }
        else
        {
            m_debug += delta;
        }
    }

};

int main(int arc, char *argv[])
{
    SandboxLayer gameLayer;

    return app().run();
}