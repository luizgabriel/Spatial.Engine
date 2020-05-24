#include <spatial/render/Resources.h>

namespace fl = filament;

namespace spatial
{

SwapChain createSwapChain(fl::Engine* engine, void* nativeWindowHandle) noexcept
{
	return createResource(engine, engine->createSwapChain(nativeWindowHandle));
}

Renderer createRenderer(fl::Engine* engine) noexcept
{
	return createResource(engine, engine->createRenderer());
}

Scene createScene(fl::Engine* engine) noexcept
{
	return createResource(engine, engine->createScene());
}

View createView(fl::Engine* engine) noexcept
{
	return createResource(engine, engine->createView());
}

Camera createCamera(fl::Engine* engine) noexcept
{
	return createResource(engine, engine->createCamera());
}

Entity createEntity(fl::Engine* engine) noexcept
{
	return {engine, utils::EntityManager::get().create()};
}

MaterialInstance createMaterialInstance(filament::Engine* engine, filament::Material* material) noexcept
{
	return createResource(engine, material->createInstance());
}

} // namespace spatial