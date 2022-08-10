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
#include <filament/Viewport.h>

#include <spatial/graphics/Camera.h>
#include <spatial/graphics/EngineResource.h>
#include <spatial/graphics/Entity.h>
#include <spatial/graphics/MaterialInstance.h>

#include <string_view>

namespace spatial::graphics
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
using SharedMaterialInstance = std::shared_ptr<graphics::MaterialInstance>;
using SharedView = SharedEngineResource<filament::View>;
using SharedTexture = SharedEngineResource<filament::Texture>;
using SharedCamera = SharedEngineResource<Camera>;
using SharedIndirectLight = SharedEngineResource<filament::IndirectLight>;
using SharedRenderTarget = SharedEngineResource<filament::RenderTarget>;

struct MeshGeometry
{
	size_t offset;
	size_t count;
};

using MeshGeometries = std::vector<MeshGeometry>;

SwapChain createSwapChain(filament::Engine& engine, void* nativeWindowHandle) noexcept;

Renderer createRenderer(filament::Engine& engine) noexcept;

Scene createScene(filament::Engine& engine) noexcept;

View createView(filament::Engine& engine) noexcept;

Camera createCamera(filament::Engine& engine, utils::Entity entity) noexcept;

Entity createEntity(filament::Engine& engine) noexcept;

VertexBuffer createVertexBuffer(filament::Engine& engine, filament::VertexBuffer::Builder builder);

IndexBuffer createIndexBuffer(filament::Engine& engine, filament::IndexBuffer::Builder builder);

MaterialInstance createMaterialInstance(filament::Engine& engine, const SharedMaterial& material,
										std::string_view = {}) noexcept;

RenderTarget createRenderTarget(filament::Engine& engine, filament::RenderTarget::Builder builder);

Material createMaterial(filament::Engine& engine, const uint8_t* data, size_t size);

Texture createTexture(filament::Engine& engine, filament::Texture::Builder builder);

} // namespace spatial::graphics