#include <spatial/render/CommonResources.h>

namespace fl = filament;

namespace spatial::render
{

SwapChain createSwapChain(fl::Engine *engine, void *nativeWindowHandle) noexcept
{
    return {engine, engine->createSwapChain(nativeWindowHandle)};
}

Renderer createRenderer(fl::Engine *engine) noexcept
{
    return {engine, engine->createRenderer()};
}

Scene createScene(fl::Engine *engine) noexcept
{
    return {engine, engine->createScene()};
}

View createView(fl::Engine *engine) noexcept
{
    return {engine, engine->createView()};
}

Camera createCamera(fl::Engine *engine) noexcept
{
    return {engine, engine->createCamera()};
}

Entity createEntity(fl::Engine *engine) noexcept
{
    return {engine, utils::EntityManager::get().create()};
}

}