#include <spatial/render/Engine.h>

#ifdef SPATIAL_DEBUG
#include <iostream>
#endif

namespace spatial::render
{

const char *gStrSwapChainResourceName = "SwapChain";
const char *gStrRendererResourceName = "Renderer";
const char *gStrSceneResourceName = "Scene";
const char *gStrViewResourceName = "View";
const char *gStrCameraResourceName = "Camera";

RenderEngine::RenderEngine(filament::backend::Backend backend)
    : m_engine{filament::Engine::create(backend)}
{
}

RenderEngine::~RenderEngine()
{
#ifdef SPATIAL_DEBUG
    std::cout << "\n[SPATIAL] Cleaning rendering engine. ";
#endif

    filament::Engine::destroy(&m_engine);
}

SwapChain RenderEngine::createSwapChain(void *nativeWindowHandle)
{
    return {m_engine, m_engine->createSwapChain(nativeWindowHandle)};
}

Renderer RenderEngine::createRenderer()
{
    return {m_engine, m_engine->createRenderer()};
}

Scene RenderEngine::createScene()
{
    return {m_engine, m_engine->createScene()};
}

View RenderEngine::createView()
{
    return {m_engine, m_engine->createView()};
}

Camera RenderEngine::createCamera()
{
    return {m_engine, m_engine->createCamera()};
}

} // namespace spatial::render