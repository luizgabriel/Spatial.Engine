#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class SceneController
{
  public:
	SceneController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
};

} // namespace spatial::render