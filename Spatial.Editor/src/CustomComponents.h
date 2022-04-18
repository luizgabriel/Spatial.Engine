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

using TextureFinder = std::function<const filament::Texture*(const ResourcePath&)>;

class EntityProperties
{
  public:
	static bool displayComponents(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons,
								  const TextureFinder& finder, bool showDebugComponents = false);

	static void popup(ecs::Registry& registry, ecs::Entity entity);

  private:
	static void addComponentMenu(ecs::Registry& registry, ecs::Entity entity);

	static bool displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity);
};

struct EditorMainMenu
{
	enum class FileMenuAction
	{
		Unknown,
		OpenProject,
		NewScene,
		OpenScene,
		SaveScene,
	};

	static FileMenuAction fileMenu(const filament::Texture& icons);
	static bool viewOptionsMenu(bool& isEditorEntitiesShowing, bool& isEditorComponentsShowing);
	static bool createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
						   const math::float3& createEntitiesPosition);
};

class NewSceneModal
{
  public:
	NewSceneModal();
	bool onConfirm();

	static void open();

	static bool show()
	{
		auto modal = NewSceneModal{};
		return modal.onConfirm();
	}

  private:
	PopupModal mModal;
	bool mIsConfirmed{false};
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

class SaveSceneModal
{
  public:
	explicit SaveSceneModal(std::filesystem::path& savePath);
	bool onConfirm();

	static void open();

	static bool show(std::filesystem::path& savePath)
	{
		auto modal = SaveSceneModal{savePath};
		return modal.onConfirm();
	}

  private:
	PopupModal mModal;
	bool mConfirmed;
};

class OpenProjectModal
{
  public:
	explicit OpenProjectModal(std::filesystem::path& openPath);
	bool onConfirm();

	static void open();

	static bool show(std::filesystem::path& openPath)
	{
		auto modal = OpenProjectModal{openPath};
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

class AssetsManager
{
  public:
	enum class AssetType
	{
		Material,
		Mesh,
		Script,
	};

	static bool header(std::string& search, AssetType& filter);
	static bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
					 AssetsManager::AssetType type, bool showEditorEntities);
};

class MaterialsManager
{
  public:
	static bool createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity);
	static bool header(std::string& search);
	static bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
					 bool showEditorEntities);
};

class EditorDragAndDrop
{
  public:
	static bool loadScene(std::filesystem::path& scenePath, ecs::Entity& selectedEntity);
	static bool loadResource(ecs::Registry& registry, ecs::Entity& selectedEntity);
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
struct ComponentInputImpl<editor::SkyBoxMaterial, const filament::Texture&>
{
	static constexpr auto sName = "SkyBox Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, const filament::Texture&, const TextureFinder&>
{
	static constexpr auto sName = "Standard Opaque Material";
	static bool draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons,
					 const TextureFinder& finder);
};

} // namespace spatial::ui