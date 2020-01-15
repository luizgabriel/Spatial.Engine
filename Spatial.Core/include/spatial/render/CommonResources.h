#pragma once

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/Texture.h>

#include <spatial/render/EngineResource.h>
#include <spatial/render/EntityResource.h>

#include <memory>

namespace spatial::render
{

//Scoped Pointers
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

//Shared Pointers
using SharedVertexBuffer = std::shared_ptr<filament::VertexBuffer>;
using SharedIndexBuffer = std::shared_ptr<filament::IndexBuffer>;
using SharedMaterial = std::shared_ptr<filament::Material>;
using SharedMaterialInstance = std::shared_ptr<filament::MaterialInstance>;
using SharedView = std::shared_ptr<filament::View>;
using SharedTexture = std::shared_ptr<filament::Texture>;
using SharedCamera = std::shared_ptr<filament::Camera>;

/**
 * \brief Creates a scoped pointer of the filament::SwapChain 
 */
SwapChain createSwapChain(filament::Engine *engine, void *nativeWindowHandle) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Renderer 
 */
Renderer createRenderer(filament::Engine *engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Scene 
 */
Scene createScene(filament::Engine *engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::View 
 */
View createView(filament::Engine *engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Camera 
 */
Camera createCamera(filament::Engine *engine) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Material 
 */
Material createMaterial(filament::Engine *engine, const char *data, size_t size) noexcept;

/**
 * \brief Creates a scoped pointer of the filament::Material 
 */
Material createMaterial(filament::Engine *engine, const std::vector<char> &data) noexcept;

/**
 * \brief Creates a scoped pointer of the utils::Entity
 */
EntityResource createEntity(filament::Engine *engine) noexcept;

/**
 * \brief Creates a shared pointer of any T-type instance of filament 
 */
template<typename T>
std::shared_ptr<T> createSharedResource(filament::Engine *engine, T* instance) noexcept
{
    return std::shared_ptr<T>{instance, [=](T* res){
        engine->destroy(res);
    }};
}

} // namespace spatial::render