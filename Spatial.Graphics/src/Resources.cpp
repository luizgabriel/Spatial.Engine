#include <spatial/graphics/Resources.h>

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


Material createMaterial(filament::Engine& engine, const uint8_t* data, size_t size)
{
	auto material = filament::Material::Builder().package(data, size).build(engine);
	return Material{engine, material};
}

VertexBuffer createVertexBuffer(filament::Engine& engine, filament::VertexBuffer::Builder builder)
{
	return VertexBuffer{engine, builder.build(engine)};
}

IndexBuffer createIndexBuffer(filament::Engine& engine, filament::IndexBuffer::Builder builder)
{
	return IndexBuffer{engine, builder.build(engine)};
}

RenderTarget createRenderTarget(filament::Engine& engine, filament::RenderTarget::Builder builder)
{
	return RenderTarget{engine, builder.build(engine)};
}

Texture createTexture(filament::Engine& engine, filament::Texture::Builder builder)
{
	return Texture{engine, builder.build(engine)};
}

} // namespace spatial::graphics