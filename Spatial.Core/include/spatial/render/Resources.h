#pragma once

#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/IndexBuffer.h>
#include <filament/IndirectLight.h>
#include <filament/Material.h>
#include <filament/RenderTarget.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/Skybox.h>
#include <filament/Texture.h>
#include <filament/TextureSampler.h>
#include <filament/VertexBuffer.h>
#include <filament/View.h>

#include <spatial/render/Camera.h>
#include <spatial/render/EngineResource.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MaterialInstance.h>
#include <spatial/resources/FilameshFile.h>

#include <memory>
#include <string_view>
#include <vector>

namespace spatial::render
{

// Scoped Pointers
using SwapChain = EngineResource<filament::SwapChain>;
using Renderer = EngineResource<filament::Renderer>;
using Scene = EngineResource<filament::Scene>;
using View = EngineResource<filament::View>;
using Material = EngineResource<filament::Material>;
using Texture = EngineResource<filament::Texture>;
using VertexBuffer = EngineResource<filament::VertexBuffer>;
using IndexBuffer = EngineResource<filament::IndexBuffer>;
using Skybox = EngineResource<filament::Skybox>;
using IndirectLight = EngineResource<filament::IndirectLight>;
using RenderTarget = EngineResource<filament::RenderTarget>;

// Shared Pointers
using SharedScene = SharedEngineResource<filament::Scene>;
using SharedVertexBuffer = SharedEngineResource<filament::VertexBuffer>;
using SharedIndexBuffer = SharedEngineResource<filament::IndexBuffer>;
using SharedMaterial = SharedEngineResource<filament::Material>;
using SharedMaterialInstance = std::shared_ptr<render::MaterialInstance>;
using SharedView = SharedEngineResource<filament::View>;
using SharedTexture = SharedEngineResource<filament::Texture>;
using SharedCamera = SharedEngineResource<filament::Camera>;

struct MeshGeometry
{
	size_t offset;
	size_t count;
};

using MeshGeometries = std::vector<MeshGeometry>;

/**
 * \brief Creates a scoped pointer of the filament::SwapChain
 */
SwapChain createSwapChain(filament::Engine& engine, void* nativeWindowHandle) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Renderer
 */
Renderer createRenderer(filament::Engine& engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Scene
 */
Scene createScene(filament::Engine& engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::View
 */
View createView(filament::Engine& engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Camera
 */
Camera createCamera(filament::Engine& engine, utils::Entity entity) noexcept;

/**
 * \brief Creates a scoped pointer of the utils::Entity
 */
Entity createEntity(filament::Engine& engine) noexcept;

MaterialInstance createMaterialInstance(filament::Engine& engine, const SharedMaterial& material,
										std::string_view = {}) noexcept;

RenderTarget createRenderTarget(filament::Engine& engine, filament::Texture& color, filament::Texture& depth);

Material createMaterial(filament::Engine& engine, const uint8_t* data, size_t size);

Texture createTexture(filament::Engine& engine, math::int2 dimensions, filament::Texture::InternalFormat format,
					  filament::Texture::Usage usage = filament::Texture::Usage::DEFAULT,
					  filament::Texture::Sampler sampler = filament::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const uint8_t* data, size_t size,
					  filament::Texture::Usage usage = filament::Texture::Usage::DEFAULT,
					  filament::Texture::Sampler sampler = filament::Texture::Sampler::SAMPLER_2D);

Texture createDummyCubemap(filament::Engine& engine);

template <uint32_t color>
Texture createDummyTexture(filament::Engine& engine)
{
	auto texture = createTexture(engine, {1, 1}, filament::Texture::InternalFormat::RGBA8,
								 filament::Texture::Usage::DEFAULT, filament::Texture::Sampler::SAMPLER_2D);

	static const uint32_t pixel = color;
	auto buffer = filament::Texture::PixelBufferDescriptor(&pixel, 4, filament::Texture::Format::RGBA,
														   filament::Texture::Type::UBYTE);

	auto bufferDescriptor = filament::Texture::PixelBufferDescriptor{&pixel, 4, filament::Texture::Format::RGBA,
																	 filament::Texture::Type::UBYTE};

	texture->setImage(engine, 0, std::move(bufferDescriptor));

	return texture;
}

VertexBuffer createVertexBuffer(filament::Engine& engine, const FilameshFile& filamesh);

IndexBuffer createIndexBuffer(filament::Engine& engine, const FilameshFile& filamesh);

VertexBuffer createFullScreenVertexBuffer(filament::Engine& engine);

IndexBuffer createFullScreenIndexBuffer(filament::Engine& engine);

MeshGeometries createMeshGeometries(const FilameshFile& filamesh);

} // namespace spatial::render