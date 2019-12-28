#pragma once

#include <filament/Engine.h>
#include <filament/View.h>
#include <filament/Renderer.h>

#include <spatial/render/EngineResource.h>
#include <memory>

namespace spatial::render
{

extern const char *gStrSwapChainResourceName;
using SwapChain = EngineResource<filament::SwapChain, &gStrSwapChainResourceName>;

extern const char *gStrRendererResourceName;
using Renderer = EngineResource<filament::Renderer, &gStrRendererResourceName>;

extern const char *gStrSceneResourceName;
using Scene = EngineResource<filament::Scene, &gStrSceneResourceName>;

extern const char *gStrViewResourceName;
using View = EngineResource<filament::View, &gStrViewResourceName>;

extern const char *gStrCameraResourceName;
using Camera = EngineResource<filament::Camera, &gStrCameraResourceName>;

class RenderEngine
{
private:
    filament::Engine *m_engine;

public:
    RenderEngine(filament::backend::Backend backend);
    ~RenderEngine();

    filament::Engine *get()
    {
        return m_engine;
    }

    filament::Engine& get_ref()
    {
        return *m_engine;
    }

    const filament::Engine *get() const
    {
        return m_engine;
    }

    const filament::Engine& get_ref() const
    {
        return *m_engine;
    }

    SwapChain createSwapChain(void *nativeWindowHandle) noexcept;
    Renderer createRenderer() noexcept;
    Scene createScene() noexcept;
    View createView() noexcept;
    Camera createCamera() noexcept;
};

} // namespace spatial::render