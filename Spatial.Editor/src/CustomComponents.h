#pragma once

#include <filesystem>
#include <spatial/ecs/Registry.h>
#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/PopupModal.h>
#include <spatial/ui/components/Window.h>

namespace spatial::ui
{

class PropertiesPanel
{
  public:
	PropertiesPanel(ecs::Registry& registry, ecs::Entity selectedEntity);

	template <typename Component>
	bool onComponent(const std::string_view componentName)
	{
		if (!mWindow.isOpen()) return false;

		auto isOpen = false;

		if (mRegistry.hasAllComponents<Component>(mSelectedEntity))
		{
			auto collapse = Collapse{componentName};
			isOpen = collapse.isOpen();

			if (collapse.onClose()) {
				mRegistry.removeComponent<Component>(mSelectedEntity);
				isOpen = false;
			}
		}

		return isOpen;
	}

  private:
	Window mWindow;
	ecs::Registry& mRegistry;
	ecs::Entity mSelectedEntity;
};

class EditorMainMenu
{
  public:
	explicit EditorMainMenu(std::filesystem::path& rootPath, std::filesystem::path& scenePath);

	bool onOpenProject();

	bool onNewScene();

	bool onOpenScene();

	bool onSaveScene();

  private:
	enum class MainMenuOption
	{
		Unknown,
		OpenProject,
		NewScene,
		OpenScene,
		SaveScene,
	};

	MenuBar mMenuBar;
	MainMenuOption mSelectedOption{MainMenuOption::Unknown};
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

  private:
	PopupModal mModal;
	bool mIsConfirmed;
};

class SceneGraphWindow
{
  public:
	SceneGraphWindow(bool& showDebugEntities);

	bool header(ecs::Registry& registry, std::string& search);

	bool listTree(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search = "");

  private:
	Window mWindow;
	bool& mShowEditorEntities;

	bool displayNode(const ecs::Registry& registry, ecs::Entity entity, ecs::Entity& selectedEntity);
};

class MaterialsWindow
{
  public:
	MaterialsWindow();

	bool header(ecs::Registry& registry, std::string& search);

	bool list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search = "");

  private:
	Window mWindow;
};

}