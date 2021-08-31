#pragma once

#include <imgui.h>

#include "EditorCameraController.h"
#include <entt/entity/entity.hpp>
#include <filament/Viewport.h>
#include <spatial/common/Math.h>
#include <spatial/desktop/InputState.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/render/CameraController.h>
#include <spatial/render/LightController.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TextureView.h>
#include <spatial/render/TransformController.h>
#include <spatial/resources/FilameshFile.h>

#include <ghc/filesystem.hpp>

namespace fl = filament;

namespace spatial::editor
{

class SceneEditorSystem
{
  private:
	filament::Engine& mEngine;
	desktop::Window& mWindow;

	render::TextureView mEditorView;
	render::Scene mEditorScene;

	render::Material mDefaultMaterial;
	render::Texture mIblTexture;
	render::Texture mSkyboxTexture;
	render::IndirectLight mSkyboxLight;
	render::Skybox mSkybox;

	ecs::Entity mSelectedEntity;

	ecs::Registry mRegistry;

	EditorCameraController mEditorCameraController;

	render::SceneController mSceneController;
	render::MaterialController mMaterialController;
	render::TransformController mTransformController;
	render::CameraController mCameraController;
	render::LightController mLightController;
	render::MeshController mMeshController;

  public:
	explicit SceneEditorSystem(filament::Engine& engine, desktop::Window& window);

	void onStart();

	void onUpdateFrame(float delta);

	void onUpdateInput(const desktop::InputState& input);

	void onDrawGui();

	void onRender(filament::Renderer& renderer) const;

	void onSceneWindowResized(const math::int2& size);

	void saveScene(const ghc::filesystem::path& outputPath);

	void loadScene(const ghc::filesystem::path& inputPath);

	void newScene();
	bool hasEditorCamera() const;
};

ecs::Entity createDefaultScene(ecs::Registry& registry);

} // namespace spatial::editor
