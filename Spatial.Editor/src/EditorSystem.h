#pragma once

#include <entt/entity/registry.hpp>
#include <spatial/assets/ResourcesLoder.h>
#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/ecs/RenderableSystem.h>
#include <spatial/ecs/TransformSystem.h>
#include <spatial/render/CameraControllers.h>
#include <spatial/render/Entity.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>

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
	filament::View& mMainView;

	Scene mScene;
	Entity mCameraEntity;
	Camera mCameraComponent;
	Material mDefaultMaterial;

	SimpleCameraView mCam;
	CameraData mCameraData;

	bool showEngineGui{true};
	bool enabledCameraController{true};

	entt::registry mRegistry;
	ecs::RenderableSystem mRenderableSystem;
	ecs::TransformSystem mTransformSystem;

public:
	EditorSystem(const assets::ResourcesLoader& resources, RenderingSystem& renderingSystem);

	void onEvent(const MouseMovedEvent& e);

	void onUpdateFrame(float delta);

	void onDrawGui();
};

} // namespace spatial
