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
	static bool displayComponents(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons,
								  bool showDebugComponents = false);

	static void popup(ecs::Registry& registry, ecs::Entity entity);

  private:
	static void addComponentMenu(ecs::Registry& registry, ecs::Entity entity);

	static bool displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity);
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

	static bool fileMenu(graphics::OptionalTexture icons, Action& action);
	static bool viewOptionsMenu(bool& isEditorEntitiesShowing, bool& isEditorComponentsShowing);
	static bool createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
						   const math::vec3& createEntitiesPosition);
};

struct EditorModals
{
	static bool newScene();
	static bool openScene(std::string& openPath);
	static bool saveScene(std::string& savePath);
	static bool openProject(std::string& openPath);
};

class SceneOptionsMenu
{
  public:
	static bool createEntitiesMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
								   math::vec3 createEntitiesPosition, bool addAsChild = false);

	static bool addChildMenu(ecs::Registry& registry, ecs::Entity& selectedEntity, math::vec3 createEntitiesPosition);
	static bool removeMenu(ecs::Registry& registry, ecs::Entity& selectedEntity);

  private:
	static bool createMeshMenu(ecs::Registry& registry, ecs::Entity& selectedEntity, math::vec3 createEntitiesPosition,
							   bool addAsChild = false);
	static bool createLightMenu(ecs::Registry& registry, ecs::Entity& selectedEntity, math::vec3 createEntitiesPosition,
								bool addAsChild = false);
	static bool createCameraMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
								 math::vec3 createEntitiesPosition, bool addAsChild = false);
};

class SceneTree
{
  public:
	static bool displayTree(const ecs::Registry& registry, ecs::Entity& selectedEntity, bool showDebugEntities = false,
							std::string_view search = "");

  private:
	static bool displayNode(const ecs::Registry& registry, ecs::Entity entity, ecs::Entity& selectedEntity);
};

class ResourceManager
{
  public:
	enum class ResourceType
	{
		All,
		Material,
		Mesh,
		Script,
		Texture,
	};

	static bool header(std::string& search, ResourceType& filter, graphics::OptionalTexture icons);
	static bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
					 ResourceManager::ResourceType type, bool showEditorEntities);
};

class MaterialsManager
{
  public:
	static bool createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity);
	static bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
					 bool showEditorEntities);
};

class EditorDragAndDrop
{
  public:
	static bool loadScene(std::string& scenePath, ecs::Entity& selectedEntity);
	static bool loadMeshInstance(ecs::Registry& registry, ecs::Entity& selectedEntity,
								 math::vec3 createEntityPosition = {});
	static bool loadScriptResource(ecs::Registry& registry, ecs::Entity& selectedEntity,
								   ResourceManager::ResourceType& type);
};

template <>
struct ComponentInputImpl<editor::ColorMaterial>
{
	static constexpr auto sName = "Color Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::EditorCamera>
{
	static constexpr auto sName = "Editor Camera";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::GridMaterial>
{
	static constexpr auto sName = "Grid Camera";
	static bool draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::SkyBoxMaterial, graphics::OptionalTexture>
{
	static constexpr auto sName = "SkyBox Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, graphics::OptionalTexture>
{
	static constexpr auto sName = "Standard Opaque Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons);
};

} // namespace spatial::ui