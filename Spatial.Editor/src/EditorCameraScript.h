#pragma once

#include "EditorCamera.h"
#include <spatial/desktop/InputState.h>
#include <spatial/desktop/Window.h>
#include <spatial/ecs/Registry.h>

namespace spatial::editor
{

class EditorCameraScript
{
  public:
	EditorCameraScript(ecs::Registry& stage, const desktop::Window& window, const desktop::InputState& inputState);

	void onStart();

	void onUpdateFrame(float delta);

	void onEditorViewResized(double aspectRatio);

	ecs::Entity getCameraInstance()
	{
		return mCamera;
	}

  private:
	ecs::Registry& mRegistry;
	const desktop::Window& mWindow;
	const desktop::InputState& mInputState;
	ecs::Entity mCamera;

	math::float3 getInputAxis();
};

void componentInput(EditorCamera& component);

} // namespace spatial::editor