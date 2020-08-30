#pragma once

#include <imgui.h>

#include <spatial/spatial.h>

#include <entt/entity/registry.hpp>
#include <spatial/ui/ImGuiSceneWindow.h>

#include <math/mat4.h>
#include <math/vec2.h>
#include <math/vec3.h>

namespace fl = filament;

namespace spatial
{

class EditorSystem
{
	struct CameraMovement
	{
		bool warpMouse;
		float sensitivity;
		float velocity;
	};

  private:
	filament::Engine& mEngine;

	SharedView mSceneView;

	Scene mScene;

	Texture mIblTexture;
	Texture mSkyboxTexture;
	IndirectLight mSkyboxLight;
	Skybox mSkybox;

	ui::ImGuiSceneWindow mImGuiSceneWindow;

	bool showEngineGui{true};

	entt::registry mRegistry;
	ecs::MeshRegistry mMeshRegistry;
	ecs::MaterialRegistry mMaterialRegistry;

	ecs::RenderableSystem mRenderableSystem;
	ecs::TransformSystem mTransformSystem;
	ecs::SceneManagerSystem mSceneManagerSystem;
	ecs::CameraSystem mCameraSystem;
	ecs::LightSystem mLightSystem;

	entt::entity mCameraEntity;
	entt::entity mSelectedEntity;

	CameraMovement mMovement;

  public:
	EditorSystem(RenderingSystem& rendering);

	EditorSystem(fl::Engine& engine);

	void onEvent(const MouseMovedEvent& e);

	void onStart();

	void onUpdateFrame(float delta);

	void onDrawGui();

	void onSceneWindowResized(ui::ImGuiSceneWindow::Size size);

  private:
	entt::entity createObject(std::string name, const std::string_view shape, math::float3 position, math::float4 color, float scale = 1.0f);
};

} // namespace spatial
