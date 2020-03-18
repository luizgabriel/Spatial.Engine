#pragma once

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/Texture.h>

#include <spatial/render/EngineRef.h>
#include <spatial/render/Entity.h>

#include <memory>

namespace spatial::render
{

// Scoped Pointers
using SwapChain = EngineRef<filament::SwapChain>;
using Renderer = EngineRef<filament::Renderer>;
using Scene = EngineRef<filament::Scene>;
using View = EngineRef<filament::View>;
using Camera = EngineRef<filament::Camera>;
using Material = EngineRef<filament::Material>;
using MaterialInstance = EngineRef<filament::MaterialInstance>;
using Texture = EngineRef<filament::Texture>;
using VertexBuffer = EngineRef<filament::VertexBuffer>;
using IndexBuffer = EngineRef<filament::IndexBuffer>;
using Skybox = EngineRef<filament::Skybox>;
using IndirectLight = EngineRef<filament::IndirectLight>;

// Shared Pointers
template <typename T>
using SharedEngineRef = std::shared_ptr<T>;

using SharedVertexBuffer = SharedEngineRef<filament::VertexBuffer>;
using SharedIndexBuffer = SharedEngineRef<filament::IndexBuffer>;
using SharedMaterial = SharedEngineRef<filament::Material>;
using SharedMaterialInstance = SharedEngineRef<filament::MaterialInstance>;
using SharedView = SharedEngineRef<filament::View>;
using SharedTexture = SharedEngineRef<filament::Texture>;
using SharedCamera = SharedEngineRef<filament::Camera>;

/**
 * \brief Creates a scoped pointer of the filament::SwapChain
 */
SwapChain createSwapChain(filament::Engine* engine, void* nativeWindowHandle) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Renderer
 */
Renderer createRenderer(filament::Engine* engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Scene
 */
Scene createScene(filament::Engine* engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::View
 */
View createView(filament::Engine* engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Camera
 */
Camera createCamera(filament::Engine* engine) noexcept;

/**
 * \brief Creates a scoped pointer of the utils::Entity
 */
Entity createEntity(filament::Engine* engine) noexcept;

/**
 * \brief Creates a shared pointer of any T-type instance of filament
 */
template <typename T>
SharedEngineRef<T> createSharedResource(filament::Engine* engine, T* instance) noexcept
{
	return {instance, [=](T* res) { engine->destroy(res); }};
}

} // namespace spatial::render