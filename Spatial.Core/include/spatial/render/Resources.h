#pragma once

#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Material.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/Texture.h>
#include <filament/IndirectLight.h>
#include <filament/Skybox.h>

#include <spatial/render/EngineResource.h>
#include <spatial/render/Entity.h>

#include <memory>

namespace spatial
{

// Scoped Pointers
using SwapChain = EngineResource<filament::SwapChain>;
using Renderer = EngineResource<filament::Renderer>;
using Scene = EngineResource<filament::Scene>;
using View = EngineResource<filament::View>;
using Camera = EngineResource<filament::Camera>;
using Material = EngineResource<filament::Material>;
using MaterialInstance = EngineResource<filament::MaterialInstance>;
using Texture = EngineResource<filament::Texture>;
using VertexBuffer = EngineResource<filament::VertexBuffer>;
using IndexBuffer = EngineResource<filament::IndexBuffer>;
using Skybox = EngineResource<filament::Skybox>;
using IndirectLight = EngineResource<filament::IndirectLight>;

// Shared Pointers
using SharedVertexBuffer = SharedEngineResource<filament::VertexBuffer>;
using SharedIndexBuffer = SharedEngineResource<filament::IndexBuffer>;
using SharedMaterial = SharedEngineResource<filament::Material>;
using SharedMaterialInstance = SharedEngineResource<filament::MaterialInstance>;
using SharedView = SharedEngineResource<filament::View>;
using SharedTexture = SharedEngineResource<filament::Texture>;
using SharedCamera = SharedEngineResource<filament::Camera>;

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

MaterialInstance createMaterialInstance(filament::Engine* engine, filament::Material* material) noexcept;

} // namespace spatial