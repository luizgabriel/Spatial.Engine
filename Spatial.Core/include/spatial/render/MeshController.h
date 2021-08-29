#pragma once

#include <filament/Engine.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <unordered_map>

namespace spatial::render
{

class MeshController
{
  public:
	struct MeshGeometry {
		std::uint32_t offset;
		std::uint32_t count;
	};

	explicit MeshController(filament::Engine& engine);

	void synchronize(ecs::Registry& registry);

	void load(std::uint32_t resourceId, const FilameshFile& filamesh);

	void unload(std::uint32_t resourceId);

  private:
	filament::Engine& mEngine;
	std::unordered_map<std::uint32_t, VertexBuffer> mVertexBuffers;
	std::unordered_map<std::uint32_t, IndexBuffer> mIndexBuffers;
	std::unordered_map<std::uint32_t, std::vector<MeshGeometry>> mMeshGeometries;
	std::unordered_map<std::uint32_t, filament::Box> mBoundingBoxes;

	void createRenderableMeshes(ecs::Registry& registry) const;
	void updateMeshGeometries(ecs::Registry& registry);
	void updateMeshMaterials(ecs::Registry& registry) const;
	bool hasMeshData(std::uint32_t resourceId) const;
	void clearDeletedMeshes(ecs::Registry& registry);
};

} // namespace spatial::render