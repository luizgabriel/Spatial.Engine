#pragma once

#include <filament/Engine.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <unordered_map>
#include <list>

namespace spatial::render
{

class MeshController
{
  public:
	using MeshId = uint32_t;
	struct MeshGeometry {
		std::uint32_t offset;
		std::uint32_t count;
	};

	explicit MeshController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry, float delta);

	void load(MeshId resourceId, const FilameshFile& filamesh);

	void setRootPath(const std::filesystem::path& root);

	void setDefaultMaterialInstance(const SharedMaterialInstance& mDefaultMaterialInstance);

  private:
	filament::Engine& mEngine;
	std::filesystem::path mRoot;
	std::unordered_map<MeshId, VertexBuffer> mVertexBuffers;
	std::unordered_map<MeshId, IndexBuffer> mIndexBuffers;
	std::unordered_map<MeshId, std::vector<MeshGeometry>> mMeshGeometries;
	std::unordered_map<MeshId, filament::Box> mBoundingBoxes;

	SharedMaterialInstance mDefaultMaterialInstance;

	void populateMeshesDatabase(ecs::Registry& registry);

	void createRenderableMeshes(ecs::Registry& registry);
	void updateMeshGeometries(ecs::Registry& registry);
	void clearDeletedOrDirtyMeshes(ecs::Registry& registry);

	[[nodiscard]] bool hasMeshData(MeshId resourceId) const;
};

} // namespace spatial::render