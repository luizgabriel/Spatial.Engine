#pragma once

#include <filament/Engine.h>
#include <list>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <unordered_map>
#include <spatial/resources/FilesSystem.h>

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
	void updateMeshGeometries(ecs::Registry& registry);
	void clearDirtyRenderables(ecs::Registry& registry);
};

} // namespace spatial::render