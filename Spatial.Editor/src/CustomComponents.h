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
	static bool displayComponents(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons,
								  const render::ImageTextureFinder& finder);

	static void popup(ecs::Registry& registry, ecs::Entity entity);

  private:
	static void addComponentMenu(ecs::Registry& registry, ecs::Entity entity);

	static void displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity);
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

	static bool fileMenu(const filament::Texture& icons);
	static bool viewOptionsMenu(bool& mShowEditorEntities);
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
	static bool popup(ecs::Registry& registry, ecs::Entity& selectedEntity);
	static bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search = "",
					 bool showEditorEntities = false);
};

class EditorDragAndDrop
{
  public:
	static bool loadScene(std::filesystem::path& scenePath, ecs::Entity& selectedEntity);
	static void loadMesh(ecs::Registry& registry, ecs::Entity& selectedEntity);
	static bool loadMeshInstance(ecs::Registry& registry, ecs::Entity& selectedEntity, math::float3 createEntityPosition = {});
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
struct ComponentInputImpl<editor::SkyBoxMaterial, const filament::Texture&>
{
	static constexpr auto sName = "SkyBox Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, const filament::Texture&, const render::ImageTextureFinder&>
{
	static constexpr auto sName = "Standard Opaque Material";
	static void draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons,
					 const render::ImageTextureFinder& finder);
};

} // namespace spatial::ui