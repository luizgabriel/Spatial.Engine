#pragma once

#include <imgui.h>

#include "ImGuiSceneWindow.h"
#include "EditorCameraSystem.h"
#include <entt/entity/entity.hpp>
#include <filament/Viewport.h>
#include <spatial/common/Math.h>
#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/InputState.h>
#include <spatial/render/FilameshFile.h>
#include <spatial/render/InstanceHandle.h>
#include <spatial/render/Stage.h>

namespace fl = filament;

namespace spatial::editor
{

class SceneEditorSystem
{
  private:
	filament::Engine& mEngine;
	desktop::Window& mWindow;
	const desktop::InputState& mInputState;
	Stage mMainStage;

	Material mDefaultMaterial;
	Texture mIblTexture;
	Texture mSkyboxTexture;
	IndirectLight mSkyboxLight;
	Skybox mSkybox;

	ImGuiSceneWindow mImGuiSceneWindow;
	InstanceHandle mSelectedInstance;

	std::array<FilameshFile, 4> mMeshes;
	std::array<MaterialInstance, 4> mMaterialInstances;

	EditorCameraSystem mCameraEditorScript;

  public:
	explicit SceneEditorSystem(filament::Engine& engine, desktop::Window& window, const desktop::InputState& inputState);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();

	void onRender(filament::Renderer& renderer) const;

	void onSceneWindowResized(const math::int2& size);
};

} // namespace spatial
