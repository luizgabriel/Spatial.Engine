#pragma once

#include <spatial/render/ResourceLoaders.h>
#include <spatial/ecs/ResourceSystem.h>
#include <spatial/ecs/Components.h>
#include <filament/Engine.h>

namespace spatial::ecs
{

using MaterialSystem = ResourceSystem<ecs::Material, spatial::Material, &createMaterial>;

} // namespace spatial::ecs
