#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class TransformController
{
  public:
	explicit TransformController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render