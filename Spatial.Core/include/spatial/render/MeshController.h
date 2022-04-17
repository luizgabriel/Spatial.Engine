#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <spatial/resources/FileSystem.h>

namespace spatial::render
{

class MeshController
{
  public:
	explicit MeshController(filament::Engine& engine, FileSystem& fileSystem);

	void onUpdateFrame(ecs::Registry& registry);

	void onStartFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
	FileSystem& mFileSystem;

	void createRenderableMeshes(ecs::Registry& registry);
	static void updateMeshGeometries(ecs::Registry& registry);
	static void clearDirtyRenderables(ecs::Registry& registry);
};

} // namespace spatial::render