#pragma once

#include "Components.h"
#include "CustomUserInterface.h"
#include "EditorCameraController.h"
#include "Jobs.h"
#include "Settings.h"

#include <filament/Viewport.h>
#include <spatial/ecs/RegistryCollection.h>
#include <spatial/input/InputState.h>
#include <spatial/script/PlatformContext.h>
#include <spatial/script/ScriptController.h>

namespace fl = filament;

namespace spatial::editor
{

class EditorSystem
{
  public:
	constexpr static auto PROJECT_DIR = "project";

	explicit EditorSystem(FileSystem& fileSystem);

	void onStart();

	void onStartFrame(float delta);

	void onUpdateFrame(float delta);

	void onUpdateInput(const desktop::InputState& input);

	void onDrawGui();

	void onPublishRegistry(ecs::RegistryCollection& publisher);

	void setRootPath(const std::filesystem::path& path);
	void setScenePath(const std::string& path);

	void clearScene();
	void loadScene();
	void saveScene();

	friend class spatial::EventQueue;

  private:
	void onEvent(const ClearSceneEvent& event);
	void onEvent(const LoadSceneEvent& event);
	void onEvent(const SaveSceneEvent& event);
	void onEvent(const OpenProjectEvent& event);

	void createDefaultEditorEntities();

	FileSystem& mFileSystem;
	script::PlatformContext mPlatformContext;

	ecs::Registry mRegistry;
	ecs::Registry mEditorRegistry;
	ecs::Entity mIconTexture{ecs::NullEntity};

	script::Isolate mIsolate;

	EventQueue mJobQueue;
	ui::EditorMainMenu::Action mMenuAction{ui::EditorMainMenu::Action::None};
	bool mIsCameraControlEnabled;
	bool mIsCameraViewWindowHovered;

	std::string mScenePath;
	std::string mCurrentPath;

	std::string getScenePath() const;
};

} // namespace spatial::editor
