#pragma once

#include <filament/Engine.h>
#include <list>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <unordered_map>

namespace spatial::render
{

class MeshController
{
  public:
	using MeshId = uint32_t;
	struct MeshGeometry
	{
		size_t offset;
		size_t count;
	};

	explicit MeshController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry);

	void load(MeshId resourceId, const FilameshFile& filamesh);

	void setRootPath(const std::filesystem::path& root);

  private:
	filament::Engine& mEngine;
	std::filesystem::path mRoot;
	std::unordered_map<MeshId, VertexBuffer> mVertexBuffers;
	std::unordered_map<MeshId, IndexBuffer> mIndexBuffers;
	std::unordered_map<MeshId, std::vector<MeshGeometry>> mMeshGeometries;
	std::unordered_map<MeshId, filament::Box> mBoundingBoxes;

	void populateMeshesDatabase(ecs::Registry& registry);
	void createRenderableMeshes(ecs::Registry& registry);
	void updateMeshGeometries(ecs::Registry& registry);
	void clearDirtyRenderables(ecs::Registry& registry);

	[[nodiscard]] bool hasMeshData(MeshId resourceId) const;
};

} // namespace spatial::render