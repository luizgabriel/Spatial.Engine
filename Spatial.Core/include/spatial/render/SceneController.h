#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class SceneController
{
  public:
	explicit SceneController(filament::Engine& engine);

	void onStartFrame(ecs::Registry& registry);

	void onUpdateFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;

	void createScenes(ecs::Registry& registry) const;
	void createRenderable(ecs::Registry& registry) const;

	static void cleanUpDestroyableEntities(ecs::Registry& registry);
	static void addToScene(ecs::Registry& registry);
};

} // namespace spatial::render