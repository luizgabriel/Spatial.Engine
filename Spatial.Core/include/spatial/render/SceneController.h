#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class SceneController
{
  public:
	static void cleanUpDestroyableEntities(ecs::Registry& registry);

	static void createScenes(filament::Engine& engine, ecs::Registry& registry);

	static void createRenderables(filament::Engine& engine, ecs::Registry& registry);

	static void organizeSceneRenderables(ecs::Registry& registry);

	static void updateScenes(ecs::Registry& registry);

	static void renderViews(filament::Renderer& renderer, ecs::Registry& registry);
};

} // namespace spatial::render