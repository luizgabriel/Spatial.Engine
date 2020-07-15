#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Resources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/CameraControllers.h>
#include <spatial/render/RenderingSystem.h>
#include <entt/entity/registry.hpp>
#include "spatial/ecs/RenderableSystem.h"
#include "spatial/ecs/TransformSystem.h"

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
	filament::Engine& mEngine;
	filament::View& mMainView;

	Scene mScene;
	Entity mCameraEntity;
	Camera mCameraComponent;

	SimpleCameraView mCam;
	CameraData mCameraData;

	bool showEngineGui{true};
	bool enabledCameraController{true};

	entt::registry mRegistry;
	ecs::RenderableSystem mRenderableSystem;
	ecs::TransformSystem mTransformSystem;

public:
	explicit EditorSystem(RenderingSystem& renderingSystem);

	void onEvent(const MouseMovedEvent& e);

	void onUpdateFrame(float delta);

	void onDrawGui();
};

} // namespace spatial
