#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/render/Resources.h>
#include <spatial/render/Entity.h>
#include <spatial/render/CameraControllers.h>
#include <spatial/render/RenderingSystem.h>
#include <entt/entity/registry.hpp>
#include <spatial/ui/ImGuiExtensions.h>
#include "DebugCubeSystem.h"
#include "spatial/ecs/RenderableSystem.h"
#include "spatial/ecs/TransformSystem.h"

namespace fl = filament;

namespace spatial
{

class Sandbox
{
	struct CameraData
	{
		float sensitivity;
		float velocity;
	};

private:
	fl::Engine* m_engine;
	fl::Camera* m_camera;
	fl::View* m_view;

	SimpleCameraView m_cam;
	CameraData m_cameraData;

	bool showEngineGui{true};
	bool enabledCameraController{true};

	Scene m_scene;
	Texture m_skyboxTexture;
	Texture m_iblTexture;
	IndirectLight m_indirectLight;
	Skybox m_skybox;

	entt::registry m_registry;
	ecs::RenderableSystem m_renderableSystem;
	ecs::DebugCubeSystem m_debugCubeSystem;
	ecs::TransformSystem m_transformSystem;
	MM::EntityEditor<entt::entity> m_editor;

public:
	explicit Sandbox(RenderingSystem& renderingSystem);

	void onEvent(const MouseMovedEvent& e);

	void onUpdateFrame(float delta);

	void onDrawGui();
};
} // namespace spatial
