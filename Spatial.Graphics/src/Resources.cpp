#include <spatial/graphics/Resources.h>

#include <filament/Fence.h>

namespace spatial::graphics
{

SwapChain createSwapChain(filament::Engine& engine, void* nativeWindowHandle) noexcept
{
	return SwapChain{engine, engine.createSwapChain(nativeWindowHandle)};
}

Renderer createRenderer(filament::Engine& engine) noexcept
{
	return Renderer{engine, engine.createRenderer()};
}

Scene createScene(filament::Engine& engine) noexcept
{
	return Scene{engine, engine.createScene()};
}

View createView(filament::Engine& engine) noexcept
{
	return View{engine, engine.createView()};
}

Camera createCamera(filament::Engine& engine, utils::Entity entity) noexcept
{
	return Camera{engine, entity};
}

Entity createEntity(filament::Engine& engine) noexcept
{
	return Entity{engine, utils::EntityManager::get().create()};
}

MaterialInstance createMaterialInstance(filament::Engine& engine, const SharedMaterial& material,
										const std::string_view name) noexcept
{
	return MaterialInstance{engine, material, material->createInstance(name.data())};
}

RenderTarget createRenderTarget(filament::Engine& engine, filament::Texture& color, filament::Texture& depth)
{
	auto target = filament::RenderTarget::Builder()
					  .texture(filament::RenderTarget::AttachmentPoint::COLOR, &color)
					  .texture(filament::RenderTarget::AttachmentPoint::DEPTH, &depth)
					  .build(engine);

	return RenderTarget{engine, target};
}

Material createMaterial(filament::Engine& engine, const uint8_t* data, size_t size)
{
	auto material = filament::Material::Builder().package(data, size).build(engine);
	return Material{engine, material};
}

} // namespace spatial::graphics