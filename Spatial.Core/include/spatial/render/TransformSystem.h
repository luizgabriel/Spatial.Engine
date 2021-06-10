#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class TransformSystem
{
  public:
	explicit TransformSystem(filament::Engine& engine);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;
	void createTransforms(ecs::Registry& registry) const;
	void updateTransforms(ecs::Registry& registry) const;
	void updateTransformsParents(ecs::Registry& registry) const;
	void clearRemovedTransforms(ecs::Registry& registry) const;
};

} // namespace spatial::render