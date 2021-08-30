#pragma once

#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class LightController
{
  public:
	explicit LightController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render