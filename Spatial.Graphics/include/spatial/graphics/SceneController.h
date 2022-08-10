#pragma once

#include <filament/Engine.h>
#include <filament/Scene.h>
#include <spatial/ecs/Registry.h>

namespace spatial::graphics
{

class SceneController
{
  public:
	static void cleanUpDestroyableEntities(ecs::Registry& registry);

	static void createScenes(filament::Engine& engine, ecs::Registry& registry);

	static void createRenderables(filament::Engine& engine, ecs::Registry& registry);

	static void organizeSceneRenderables(ecs::Registry& registry);

	static void updateScenes(const ecs::Registry& registry);

	static void renderViews(filament::Renderer& renderer, const ecs::Registry& registry);
};

} // namespace spatial::graphics