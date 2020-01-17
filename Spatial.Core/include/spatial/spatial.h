#pragma once

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>
#include <spatial/core/ApplicationConnector.h>
#include <spatial/core/Asset.h>

#include <spatial/render/RenderingSystem.h>
#include <spatial/render/UserInterfaceSystem.h>
#include <spatial/render/ResourceLoaders.h>

#include <spatial/input/InputSystem.h>
#include <spatial/input/Input.h>

#include <utils/EntityManager.h>
#include <filament/TransformManager.h>
#include <filament/RenderableManager.h>
#include <filament/LightManager.h>
#include <filament/Color.h>

#include <filameshio/MeshReader.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace spatial
{
    namespace math = filament::math;
}

namespace filament
{
    namespace utils = ::utils;
}