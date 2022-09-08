#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::ui
{

class EntityProperties
{
  public:
	static void displayComponents(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons,
								  bool showDebugComponents = false);

	static void popup(ecs::Registry& registry, ecs::Entity entity);

  private:
	static void addComponentMenu(ecs::Registry& registry, ecs::Entity entity);

	static void displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity);
};

} // namespace spatial::ui