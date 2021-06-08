#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>
#include <unordered_map>

namespace spatial::render
{

class RenderableMeshSystem
{
  public:
	struct VertexIndexBufferPair
	{
		ecs::Entity vertexBufferId;
		ecs::Entity indexBufferId;
	};

	RenderableMeshSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;

	void createRenderableMeshes(ecs::Registry& registry) const;
	void updateMeshGeometries(ecs::Registry& registry) const;
	void updateMeshMaterials(ecs::Registry& registry) const;
};

} // namespace spatial::render