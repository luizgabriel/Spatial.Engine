#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/InputState.h>
#include <spatial/render/InstanceHandle.h>

namespace spatial::editor
{

class EditorCameraScript
{
  public:
	EditorCameraScript(Stage& stage, desktop::Window& window, const desktop::InputState& inputState);

	void onStart();

	void onUpdateFrame(float delta);

	auto getCameraInstance()
	{
		return mCamera;
	}

  private:
	Stage& mStage;
	desktop::Window& mWindow;
	const desktop::InputState& mInputState;
	InstanceHandle mCamera;

	math::float3 getInputAxis();
};

} // namespace spatial::editor