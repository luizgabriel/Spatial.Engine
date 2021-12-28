#pragma once

#include <filesystem>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/Popup.h>
#include <spatial/ui/components/PopupModal.h>
#include <spatial/ui/components/Window.h>

namespace spatial::ui
{

class EntityProperties
{
  public:
	static bool displayComponents(ecs::Registry& registry, ecs::Entity selectedEntity);

	static void popup(ecs::Registry& registry, ecs::Entity selectedEntity);

  private:
	static void addComponentMenu(ecs::Registry& registry, ecs::Entity selectedEntity);

	static bool displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity);
	static bool displayEntityCoreComponents(ecs::Registry& registry, ecs::Entity selectedEntity);
	static bool displayEntityEditorComponents(ecs::Registry& registry, ecs::Entity selectedEntity);

	template <typename Component>
	static bool displayComponent(const std::string_view componentName, ecs::Registry& registry,
								 ecs::Entity selectedEntity)
	{
		auto isOpen = false;

		if (registry.hasAllComponents<Component>(selectedEntity))
		{
			auto collapse = Collapse{componentName};
			isOpen = collapse.isOpen();

			if (collapse.onClose())
			{
				registry.removeComponent<Component>(selectedEntity);
				isOpen = false;
			}
		}

		if (isOpen) {
			spacing(3);
			componentInput<Component>(registry, selectedEntity);
			spacing(3);
		}

		return isOpen;
	}
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

	static bool fileMenu();
	static bool viewOptionsMenu(bool& mShowEditorEntities);
};

class NewSceneModal
{
  public:
	NewSceneModal();
	bool onConfirm();

	static void open();

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

	template <typename Function>
	static void onConfirm(std::filesystem::path& openPath, Function func)
	{
		auto modal = OpenSceneModal{openPath};
		if (modal.onConfirm())
			std::invoke(std::forward<Function>(func), std::as_const(openPath));
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

	template <typename Function>
	static void onConfirm(std::filesystem::path& openPath, Function func)
	{
		auto modal = OpenProjectModal{openPath};
		if (modal.onConfirm())
			std::invoke(std::forward<Function>(func), std::as_const(openPath));
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
	static bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search = "", bool showEditorEntities = false);
};

class EditorDragAndDrop
{
  public:
	static bool loadScene(std::filesystem::path& scenePath, ecs::Entity& selectedEntity);
	static bool loadMesh(ecs::Registry& registry, ecs::Entity& selectedEntity, math::float3 createEntityPosition = {});
};

} // namespace spatial::ui