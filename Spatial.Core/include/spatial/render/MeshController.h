#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <spatial/resources/FileSystem.h>

namespace spatial::render
{

class MeshController
{
  public:
	static void loadMeshes(filament::Engine& engine, FileSystem& fileSystem, ecs::Registry& registry);

	static void createMeshInstances(filament::Engine& engine, ecs::Registry& registry);

	static void updateMeshInstances(ecs::Registry& registry);

  private:
	static void updateMeshGeometries(ecs::Registry& registry);
	static void clearDirtyRenderables(ecs::Registry& registry);
};

} // namespace spatial::render