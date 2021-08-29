#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class SceneController
{
  public:
	SceneController(filament::Engine& engine, filament::Scene& scene);

	void synchronize(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;
	filament::Scene& mScene;
};

} // namespace spatial::render