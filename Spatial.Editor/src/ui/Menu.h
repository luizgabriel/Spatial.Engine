#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::ui
{

struct EditorMainMenu
{
	enum class Action
	{
		None,
		OpenProject,
		NewScene,
		OpenScene,
		SaveScene,
	};

	static void fileMenu(graphics::OptionalTexture icons, Action& action);
	static void viewOptionsMenu(bool& isEditorEntitiesShowing, bool& isEditorComponentsShowing);
	static void createMenu(ecs::Registry& registry, const math::vec3& createEntitiesPosition);
};

struct CreateMenu
{
	static void createMeshMenu(ecs::Registry& registry, math::vec3 createEntitiesPosition);

	static void createLightMenu(ecs::Registry& registry, math::vec3 createEntitiesPosition);

	static void createCameraMenu(ecs::Registry& registry, math::vec3 createEntitiesPosition);

	static void createMaterialsMenu(ecs::Registry& registry);

	static void createViewMenu(ecs::Registry& registry);

	static void removeMenu(ecs::Registry& registry);
};

} // namespace spatial::ui