#pragma once

#include "Components.h"
#include "Materials.h"
#include <filesystem>
#include <imgui.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Popup.h>
#include <spatial/ui/components/PopupModal.h>
#include <string_view>

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

struct EditorModals
{
	static bool newScene();
	static bool openScene(std::string& openPath);
	static bool saveScene(std::string& savePath);
	static bool openProject(std::string& openPath);
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

class SceneTree
{
  public:
	static void displayTree(ecs::Registry& registry, bool showDebugEntities = false, std::string_view search = "");

  private:
	static void displayNode(ecs::Registry& registry, ecs::Entity entity);
};

struct ResourceManager
{
	enum class ResourceType
	{
		All,
		Material,
		Mesh,
		Script,
		Texture,
	};

	static void header(std::string& search, ResourceType& filter, graphics::OptionalTexture icons);
	static void list(ecs::Registry& registry, std::string_view search, ResourceManager::ResourceType type,
					 bool showEditorEntities);
};

struct MaterialsManager
{
	static void list(ecs::Registry& registry, std::string_view search, bool showEditorEntities);
};

struct ViewsManager
{
	static void list(ecs::Registry& registry, std::string_view search, bool showEditorEntities);
};

struct EditorDragAndDrop
{
	static bool loadScene(std::string& scenePath);
	static void loadMeshInstance(ecs::Registry& registry, math::vec3 createEntityPosition = {});
	static void loadScriptResource(ecs::Registry& registry, ResourceManager::ResourceType& type);
};

template <>
struct ComponentInputImpl<editor::ColorMaterial>
{
	static constexpr auto sName = "Color Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::EditorCamera>
{
	static constexpr auto sName = "Editor Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::GridMaterial>
{
	static constexpr auto sName = "Grid Camera";
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::SkyBoxMaterial, graphics::OptionalTexture>
{
	static constexpr auto sName = "SkyBox Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, graphics::OptionalTexture>
{
	static constexpr auto sName = "Standard Opaque Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

} // namespace spatial::ui