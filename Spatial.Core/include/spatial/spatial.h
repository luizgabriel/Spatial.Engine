#pragma once

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>
#include <spatial/core/ApplicationConnector.h>
#include <spatial/core/Asset.h>

#include <spatial/render/RenderingSubsystem.h>
#include <spatial/input/InputSubsystem.h>
#include <spatial/input/Input.h>

#include <utils/EntityManager.h>
#include <filament/LightManager.h>
#include <filament/Color.h>

namespace spatial
{
    namespace math = filament::math;
}

namespace filament
{
    namespace utils = ::utils;
}