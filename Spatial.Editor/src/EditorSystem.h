#pragma once

#include <entt/entity/registry.hpp>
#include <imgui.h>
#include <spatial/assets/ResourcesLoader.h>
#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/ecs/RenderableSystem.h>
#include <spatial/ecs/TransformSystem.h>
#include <spatial/render/CameraControllers.h>
#include <spatial/render/Entity.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>
#include "ShapeSystem.h"

namespace fl = filament;

namespace spatial
{

class EditorSystem
{
	struct CameraData
	{
		float sensitivity;
		float velocity;
	};

  private:
	const assets::ResourcesLoader& mResources;
	filament::Engine& mEngine;

	SharedView mSceneView;

	Texture mRenderColorTexture;
	Texture mRenderDepthTexture;
	RenderTarget mRenderTarget;

	Scene mScene;
	Entity mCameraEntity;
	Camera mCameraComponent;

	Texture mIblTexture;
	Texture mSkyboxTexture;
	IndirectLight mSkyboxLight;
	Skybox mSkybox;

	SimpleCameraView mCam;
	CameraData mCameraData;

	bool showEngineGui{true};
	bool enabledCameraController{true};

	entt::registry mRegistry;
	ecs::RenderableSystem mRenderableSystem;
	ecs::TransformSystem mTransformSystem;
	ecs::ShapeSystem mShapeSystem;

  public:
	EditorSystem(RenderingSystem& rendering, const assets::ResourcesLoader& resources);

	EditorSystem(fl::Engine& engine, const assets::ResourcesLoader& resources);

	void onEvent(const MouseMovedEvent& e);

	void onUpdateFrame(float delta);

	void onDrawGui();
	void refreshMainViewSize(const ImVec2& size);
};

} // namespace spatial
