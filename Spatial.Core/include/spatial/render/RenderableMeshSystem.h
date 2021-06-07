#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class RenderableMeshSystem
{
  public:
	RenderableMeshSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;

	void createRenderableMeshes(ecs::Registry& registry) const;
	void updateMeshGeometries(ecs::Registry& registry) const;
	void updateMeshMaterials(ecs::Registry& registry) const;
};

}