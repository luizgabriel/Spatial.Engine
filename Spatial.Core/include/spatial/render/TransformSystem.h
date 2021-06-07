#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class TransformSystem
{
  public:
	explicit TransformSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
	void createTransforms(ecs::Registry& registry) const;
	void updateTransforms(ecs::Registry& registry);
	void updateTransformsParents(ecs::Registry& registry);
};

}