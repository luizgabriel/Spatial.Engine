#pragma once

#include <spatial/render/ResourceLoaders.h>
#include <spatial/ecs/ResourceSystem.h>
#include <spatial/ecs/Components.h>
#include <filament/Engine.h>

namespace spatial::ecs
{

using MeshSystem = ResourceSystem<ecs::Mesh, spatial::Mesh, &createMesh>;

} // namespace spatial::ecs
