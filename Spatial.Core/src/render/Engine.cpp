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
const char *gStrMaterialResourceName = "Material";
const char *gStrMaterialInstanceResourceName = "Material Instance";

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

SwapChain RenderEngine::createSwapChain(void *nativeWindowHandle) noexcept
{
    return {m_engine, m_engine->createSwapChain(nativeWindowHandle)};
}

Renderer RenderEngine::createRenderer() noexcept
{
    return {m_engine, m_engine->createRenderer()};
}

Scene RenderEngine::createScene() noexcept
{
    return {m_engine, m_engine->createScene()};
}

View RenderEngine::createView() noexcept
{
    return {m_engine, m_engine->createView()};
}

Camera RenderEngine::createCamera() noexcept
{
    return {m_engine, m_engine->createCamera()};
}

Material RenderEngine::createMaterial(const std::string& data)
{
    return {
        m_engine,
        filament::Material::Builder()
            .package(data.c_str(), data.size())
            .build(*m_engine)
    };
}

MaterialInstance RenderEngine::createInstance(Material& material)
{
    return {
        m_engine,
        material->createInstance()
    };
}

} // namespace spatial::render