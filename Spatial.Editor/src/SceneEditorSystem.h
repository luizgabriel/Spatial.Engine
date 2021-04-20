#pragma once

#include <imgui.h>

#include "ImGuiSceneWindow.h"
#include <entt/entity/entity.hpp>
#include <filament/Viewport.h>
#include <spatial/common/Math.h>
#include <spatial/desktop/PlatformEvent.h>
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

	filament::Viewport mViewport;

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

  public:
	explicit SceneEditorSystem(filament::Engine& engine);

	void setViewport(const std::pair<int, int> windowSize);

	void onEvent(const WindowResizedEvent& e);

	void onEvent(const MouseMovedEvent& e);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();

	void onRender(filament::Renderer& renderer) const;

	void onSceneWindowResized(ImGuiSceneWindow::Size size);
};

} // namespace spatial
