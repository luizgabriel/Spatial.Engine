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

	explicit MeshController(filament::Engine& engine, std::filesystem::path root);

	void onUpdateFrame(ecs::Registry& registry, float delta);

	static uint32_t toMeshId(const std::filesystem::path& path);

  private:
	filament::Engine& mEngine;
	std::filesystem::path mRoot;
	std::unordered_map<MeshId, VertexBuffer> mVertexBuffers;
	std::unordered_map<MeshId, IndexBuffer> mIndexBuffers;
	std::unordered_map<MeshId, std::vector<MeshGeometry>> mMeshGeometries;
	std::unordered_map<MeshId, filament::Box> mBoundingBoxes;

	void loadMeshes(ecs::Registry& registry);

	void load(MeshId resourceId, const FilameshFile& filamesh);

	void createRenderableMeshes(ecs::Registry& registry);
	void updateMeshGeometries(ecs::Registry& registry);
	void clearDeletedMeshes(ecs::Registry& registry);

	[[nodiscard]] bool hasMeshData(MeshId resourceId) const;
};

} // namespace spatial::render