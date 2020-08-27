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
	ecs::TransformSystem mTransformSystem;
	ecs::MeshSystem mMeshSystem;
	ecs::MaterialSystem mMaterialSystem;
	ecs::RenderableSystem mRenderableSystem;
	ecs::CameraSystem mCameraSystem;

	entt::entity mCameraEntity;

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
    entt::entity createObject(std::string name, std::string resourcePath, filament::math::float3 position, filament::math::float3 scale);
};

} // namespace spatial
