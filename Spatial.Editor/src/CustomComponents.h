#pragma once

#include "EditorCamera.h"
#include "Materials.h"
#include <filesystem>
#include <imgui.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/Popup.h>
#include <spatial/ui/components/PopupModal.h>
#include <spatial/ui/components/Window.h>
#include <string_view>

namespace spatial::ui
{

template <typename Component, typename... Args>
static void componentCollapse(const std::string_view componentName, ecs::Registry& registry, ecs::Entity entity,
							  Args&&... args)
{
	if (registry.hasAllComponents<Component>(entity))
	{
		auto collapse = Collapse{componentName};
		if (collapse.isOpen())
		{
			spacing(3);
			componentInput<Component>(registry, entity, std::forward<Args>(args)...);
			spacing(3);
		}

		if (collapse.onClose())
			registry.removeComponent<Component>(entity);
	}
}

class EntityProperties
{
  public:
	static bool displayComponents(ecs::Registry& registry, ecs::Entity entity);

	static void popup(ecs::Registry& registry, ecs::Entity entity);

	static void displayEntityEditorComponents(ecs::Registry& registry, ecs::Entity entity,
											  const filament::Texture& icons, const render::ImageTextureFinder& finder);

  private:
	static void addComponentMenu(ecs::Registry& registry, ecs::Entity selectedEntity);

	static void displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity);

	static void displayEntityCoreComponents(ecs::Registry& registry, ecs::Entity selectedEntity);
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

class MaterialsManager
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
	static bool loadMesh(ecs::Registry& registry, ecs::Entity& selectedEntity, math::float3 createEntityPosition = {});
};

template <>
struct ComponentInputImpl<editor::ColorMaterial>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::EditorCamera>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::GridMaterial>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity);
};

template <>
struct ComponentInputImpl<editor::SkyBoxMaterial, const filament::Texture&>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons);
};

template <>
struct ComponentInputImpl<editor::StandardOpaqueMaterial, const filament::Texture&, const render::ImageTextureFinder&>
{
	static void draw(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons,
					 const render::ImageTextureFinder& finder);
};

} // namespace spatial::ui