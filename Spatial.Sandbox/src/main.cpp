#include <spatial/spatial.h>
#include <iostream>
#include <fmt/format.h>

using namespace spatial;

Application& app()
{
    static Application application;
    return application;
}

render::RenderEngine& engine()
{
    return app().getRenderSys().getEngine();
}

const desktop::Window& window()
{
    return app().getRenderSys().getWindow();
}

class SandboxGame
{
private:
    float m_debug = 0;

    render::Scene m_scene;
    render::Camera m_camera;

public:
    SandboxGame()
        : m_scene{engine().createScene()},
          m_camera{engine().createCamera()}
    {
        app().onStartEvent.connect<&SandboxGame::onStart>(this);
        app().onUpdateEvent.connect<&SandboxGame::onUpdate>(this);
    }

    ~SandboxGame()
    {
        app().onStartEvent.disconnect<&SandboxGame::onStart>(this);
        app().onUpdateEvent.disconnect<&SandboxGame::onUpdate>(this);
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
    SandboxGame game;

    return app().run();
}