#pragma once

#include "EditorCamera.h"
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
	static bool displayComponents(ecs::Registry& registry, ecs::Entity entity, const filament::Texture* icons, bool showDebugComponents = false);

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

	static bool fileMenu(const filament::Texture* icons, Action& action);
	static bool viewOptionsMenu(bool& isEditorEntitiesShowing, bool& isEditorComponentsShowing);
	static bool createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
						   const math::float3& createEntitiesPosition);
};

struct EditorModals
{
  static bool newScene();
  static bool openScene(std::filesystem::path& openPath);
  static bool saveScene(std::filesystem::path& savePath);
  static bool openProject(std::filesystem::path& openPath);
};

class OpenSceneModal
{
  public:
	explicit OpenSceneModal(std::filesystem::path& openPath);
	bool onConfirm();

	static void open();

	static bool show(std::filesystem::path& openPath)
	{
		auto modal = OpenSceneModal{openPath};
		return modal.onConfirm();
	}

  private:
	PopupModal mModal;
	bool mIsConfirmed;
};

class SceneOptionsMenu
{
  public:
	static bool createEntitiesMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
								   math::float3 createEntitiesPosition, bool addAsChild = false);

	static bool addChildMenu(ecs::Registry& registry, ecs::Entity& selectedEntity, math::float3 createEntitiesPosition);
	static bool removeMenu(ecs::Registry& registry, ecs::Entity& selectedEntity);

  private:
	static bool createMeshMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
							   math::float3 createEntitiesPosition, bool addAsChild = false);
	static bool createLightMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
								math::float3 createEntitiesPosition, bool addAsChild = false);
	static bool createCameraMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
								 math::float3 createEntitiesPosition, bool addAsChild = false);
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

	static bool header(std::string& search, ResourceType& filter, const filament::Texture* icons);
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
	static bool loadScene(std::filesystem::path& scenePath, ecs::Entity& selectedEntity);
	static bool loadMeshInstance(ecs::Registry& registry, ecs::Entity& selectedEntity,
								 math::float3 createEntityPosition = {});
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
struct ComponentInputImpl<editor::SkyBoxMaterial, const filament::Texture*>
{
	static constexpr auto sName = "SkyBox Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture* icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, const filament::Texture*>
{
	static constexpr auto sName = "Standard Opaque Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture* icons);
};

} // namespace spatial::ui