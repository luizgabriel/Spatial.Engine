#include <spatial/render/CommonResources.h>

namespace spatial::render
{

SwapChain createSwapChain(filament::Engine *engine, void *nativeWindowHandle) noexcept
{
    return {engine, engine->createSwapChain(nativeWindowHandle)};
}

Renderer createRenderer(filament::Engine *engine) noexcept
{
    return {engine, engine->createRenderer()};
}

Scene createScene(filament::Engine *engine) noexcept
{
    return {engine, engine->createScene()};
}

View createView(filament::Engine *engine) noexcept
{
    return {engine, engine->createView()};
}

Camera createCamera(filament::Engine *engine) noexcept
{
    return {engine, engine->createCamera()};
}

Material createMaterial(filament::Engine *engine, const char *data, size_t size) noexcept
{
    auto material = filament::Material::Builder()
                        .package(data, size)
                        .build(*engine);

    return {engine, material};
}

Material createMaterial(filament::Engine *engine, const std::vector<char> &data) noexcept
{
    return createMaterial(engine, &data[0], data.size());
}

EntityResource createEntity(filament::Engine *engine) noexcept
{
    return EntityResource{engine};
}

}