#pragma once

#include <filament/Engine.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <unordered_map>

namespace spatial::render
{

class RenderableMeshSystem
{
  public:
	struct MeshGeometry {
		std::uint32_t offset;
		std::uint32_t count;
	};

	RenderableMeshSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry);

	void load(const HashedString& resourceName, const FilameshFile& filamesh);

	void unload(const HashedString& resourceName);

  private:
	filament::Engine& mEngine;
	std::unordered_map<HashedString::hash_type, VertexBuffer> mVertexBuffers;
	std::unordered_map<HashedString::hash_type, IndexBuffer> mIndexBuffers;
	std::unordered_map<HashedString::hash_type, std::vector<MeshGeometry>> mMeshGeometries;
	std::unordered_map<HashedString::hash_type, filament::Box> mBoundingBoxes;

	void createRenderableMeshes(ecs::Registry& registry) const;
	void updateMeshGeometries(ecs::Registry& registry);
	void updateMeshMaterials(ecs::Registry& registry) const;
	bool hasMeshData(HashedString::hash_type resourceId) const;
	void clearDeletedMeshes(ecs::Registry& registry);
};

} // namespace spatial::render