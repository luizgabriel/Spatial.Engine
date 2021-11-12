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

	void onUpdateFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
	filament::Scene& mScene;
	void addToScene(ecs::Registry& registry);
	void removeFromScene(ecs::Registry& registry);
};

} // namespace spatial::render