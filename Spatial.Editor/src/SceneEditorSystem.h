#pragma once

#include <imgui.h>

#include "EditorCamera.h"
#include "EditorCameraController.h"
#include "Settings.h"

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

namespace fl = filament;

namespace spatial::editor
{

class SceneEditorSystem
{
  private:
	filament::Engine& mEngine;
	desktop::Window& mWindow;

	render::Material mDefaultMaterial;
	render::Texture mIconTexture;

	// TODO: Handle multiple scenes
	// std::vector<ecs::Registry> mOpenedScenes;
	// std::size_t mCurrentSelectedScene;

	ecs::Registry mRegistry;

	EditorCameraController mEditorCameraController;

	render::SceneController mSceneController;
	render::MaterialController mMaterialController;
	render::TransformController mTransformController;
	render::CameraController mCameraController;
	render::LightController mLightController;
	render::MeshController mMeshController;
	render::IndirectLightController mIndirectLightController;

	std::filesystem::path mRootPath;
	std::filesystem::path mScenePath;

	bool mIsReloadSceneFlagEnabled;
	bool mIsClearSceneFlagEnabled;
	bool isSaveSceneFlagEnabled;

  public:
	SceneEditorSystem(filament::Engine& engine, desktop::Window& window);

	void onStart();

	void onStartFrame(float delta);

	void onUpdateFrame(float delta);

	void onUpdateInput(const desktop::InputState& input);

	void onDrawGui();

	void onRender(filament::Renderer& renderer) const;

	void setRootPath(const std::filesystem::path& path);
};

} // namespace spatial::editor
