#pragma once

#include <imgui.h>

#include "EditorCamera.h"
#include "EditorCameraController.h"
#include "Settings.h"
#include "Jobs.h"

#include <filament/Viewport.h>
#include <spatial/common/Math.h>
#include <spatial/desktop/InputState.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <spatial/render/CameraController.h>
#include <spatial/render/IndirectLightController.h>
#include <spatial/render/LightController.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TextureView.h>
#include <spatial/render/TransformController.h>
#include <spatial/resources/PhysicalFileSystem.h>

namespace fl = filament;

namespace spatial::editor
{

class SceneEditorSystem
{
  private:
	filament::Engine& mEngine;
	desktop::Window& mWindow;

	render::Material mStandardLitMaterial;
	render::Material mColorMaterial;
	render::Material mSkyBoxMaterial;
	render::Material mGridMaterial;
	render::Texture mIconTexture;

	FileSystem& mFileSystem;
	std::shared_ptr<PhysicalFileSystem> mProjectFileSystem;

	ecs::Registry mRegistry;

	EditorCameraController mEditorCameraController;

	render::SceneController mSceneController;
	render::MaterialController mMaterialController;
	render::TransformController mTransformController;
	render::CameraController mCameraController;
	render::LightController mLightController;
	render::MeshController mMeshController;
	render::IndirectLightController mIndirectLightController;

	EventQueue mJobQueue;

	std::filesystem::path mScenePath;
	std::filesystem::path mCurrentPath;

  public:
	SceneEditorSystem(filament::Engine& engine, desktop::Window& window, FileSystem& fileSystem);

	void onStart();

	void onStartFrame(float delta);

	void onUpdateFrame(float delta);

	void onUpdateInput(const desktop::InputState& input);

	void onDrawGui();

	void onRender(filament::Renderer& renderer) const;

	void setRootPath(const std::filesystem::path& path);
	void setScenePath(const std::filesystem::path& path);

	void clearScene();
	void loadScene();
	void saveScene();

	friend class spatial::EventQueue;

  private:
	void onEvent(const ClearSceneEvent& event);
	void onEvent(const LoadSceneEvent& event);
	void onEvent(const SaveSceneEvent& event);
	void onEvent(const OpenProjectEvent& event);

};

} // namespace spatial::editor
