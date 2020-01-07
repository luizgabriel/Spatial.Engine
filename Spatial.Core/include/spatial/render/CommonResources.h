#pragma once

#include <filament/Engine.h>
#include <filament/Material.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>

#include <spatial/render/EngineResource.h>
#include <spatial/render/EntityResource.h>

namespace spatial::render
{

using SwapChain = EngineResource<filament::SwapChain>;
using Renderer = EngineResource<filament::Renderer>;
using Scene = EngineResource<filament::Scene>;
using View = EngineResource<filament::View>;
using Camera = EngineResource<filament::Camera>;
using Material = EngineResource<filament::Material>;
using MaterialInstance = EngineResource<filament::MaterialInstance>;

SwapChain createSwapChain(filament::Engine *engine, void *nativeWindowHandle) noexcept;

Renderer createRenderer(filament::Engine *engine) noexcept;

Scene createScene(filament::Engine *engine) noexcept;

View createView(filament::Engine *engine) noexcept;

Camera createCamera(filament::Engine *engine) noexcept;

Material createMaterial(filament::Engine *engine, const char *data, size_t size) noexcept;

Material createMaterial(filament::Engine *engine, const std::vector<char> &data) noexcept;

EntityResource createEntity(filament::Engine *engine) noexcept;

} // namespace spatial::render