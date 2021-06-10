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

	void toggleControl();

	void onUpdateFrame(float delta);

	void onEditorViewResized(double aspectRatio);

  private:
	ecs::Registry& mRegistry;
	const desktop::Window& mWindow;
	const desktop::InputState& mInputState;

	math::float3 getInputAxis();
	void createCamera();
};

} // namespace spatial::editor