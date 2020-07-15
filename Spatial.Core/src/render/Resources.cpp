#include <spatial/render/Resources.h>

namespace fl = filament;

namespace spatial
{

SwapChain createSwapChain(fl::Engine& engine, void* nativeWindowHandle) noexcept
{
	return SwapChain{engine, engine.createSwapChain(nativeWindowHandle)};
}

Renderer createRenderer(fl::Engine& engine) noexcept
{
	return Renderer{engine, engine.createRenderer()};
}

Scene createScene(fl::Engine& engine) noexcept
{
	return Scene{engine, engine.createScene()};
}

View createView(fl::Engine& engine) noexcept
{
	return View{engine, engine.createView()};
}

Camera createCamera(fl::Engine& engine, utils::Entity entity) noexcept
{
	return Camera{engine, entity};
}

Entity createEntity(fl::Engine& engine) noexcept
{
	return Entity{engine, utils::EntityManager::get().create()};
}

MaterialInstance createMaterialInstance(fl::Engine& engine, fl::Material& material, const char* name) noexcept
{
	return MaterialInstance{engine, material.createInstance(name)};
}

} // namespace spatial