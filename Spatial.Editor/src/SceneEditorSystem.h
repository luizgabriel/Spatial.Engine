#pragma once

#include <imgui.h>

#include "EditorCameraScript.h"
#include "ImGuiSceneWindow.h"
#include <entt/entity/entity.hpp>
#include <filament/Viewport.h>
#include <spatial/common/Math.h>
#include <spatial/desktop/InputState.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/render/CameraSystem.h>
#include <spatial/render/FilameshFile.h>
#include <spatial/render/LightSystem.h>
#include <spatial/render/MaterialInstancesSystem.h>
#include <spatial/render/RenderableMeshSystem.h>
#include <spatial/render/SceneControllerSystem.h>
#include <spatial/render/TransformSystem.h>

namespace fl = filament;

namespace spatial::editor
{

class SceneEditorSystem
{
  private:
	filament::Engine& mEngine;
	desktop::Window& mWindow;
	const desktop::InputState& mInputState;

	ecs::Registry mMainStageRegistry;

	render::View mEditorView;
	render::Scene mEditorScene;

	render::SceneControllerSystem mSceneControllerSystem;
	render::MaterialInstancesSystem mMaterialInstancesSystem;
	render::TransformSystem mTransformSystem;
	render::CameraSystem mCameraSystem;
	render::LightSystem mLightSystem;
	render::RenderableMeshSystem mRenderableMeshSystem;

	render::Material mDefaultMaterial;
	render::Texture mIblTexture;
	render::Texture mSkyboxTexture;
	render::IndirectLight mSkyboxLight;
	render::Skybox mSkybox;

	ImGuiSceneWindow mImGuiSceneWindow;
	ecs::Entity mSelectedEntity;

	std::array<render::FilameshFile, 4> mMeshes;

	EditorCameraScript mCameraEditorScript;

  public:
	explicit SceneEditorSystem(filament::Engine& engine, desktop::Window& window, const desktop::InputState& inputState);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();

	void onRender(filament::Renderer& renderer) const;

	void onSceneWindowResized(const math::int2& size);
};

} // namespace spatial
