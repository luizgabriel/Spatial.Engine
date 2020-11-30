#pragma once

#include <imgui.h>

#include <entt/entity/entity.hpp>
#include <filament/Viewport.h>
#include <spatial/common/Math.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Stage.h>
#include "ImGuiSceneWindow.h"

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
	Actor mSelectedActor;

  public:
	SceneEditorSystem(RenderingSystem& rendering);

	void setViewport(const std::pair<int, int> windowSize);

	void onEvent(const WindowResizedEvent& e);

	void onEvent(const MouseMovedEvent& e);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();

	void onSceneWindowResized(ImGuiSceneWindow::Size size);

  private:
	entt::entity createObject(std::string name, const std::string_view shape, math::float3 position, math::float4 color, float scale = 1.0f);
};

} // namespace spatial
