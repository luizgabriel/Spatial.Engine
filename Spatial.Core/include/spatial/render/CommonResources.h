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
using SharedMaterialInstance = std::shared_ptr<filament::MaterialInstance>;

SwapChain createSwapChain(filament::Engine *engine, void *nativeWindowHandle) noexcept;

Renderer createRenderer(filament::Engine *engine) noexcept;

Scene createScene(filament::Engine *engine) noexcept;

View createView(filament::Engine *engine) noexcept;

Camera createCamera(filament::Engine *engine) noexcept;

Material createMaterial(filament::Engine *engine, const char *data, size_t size) noexcept;

Material createMaterial(filament::Engine *engine, const std::vector<char> &data) noexcept;

EntityResource createEntity(filament::Engine *engine) noexcept;

template<typename T>
std::shared_ptr<T> createSharedResource(filament::Engine *engine, T* instance)
{
    return std::shared_ptr<T>{instance, [=](T* res){
        engine->destroy(res);
    }};
}

} // namespace spatial::render