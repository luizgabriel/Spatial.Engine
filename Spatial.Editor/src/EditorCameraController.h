#pragma once

#include "EditorCamera.h"
#include <spatial/desktop/InputState.h>
#include <spatial/desktop/Window.h>
#include <spatial/ecs/Registry.h>

namespace spatial::editor
{

class EditorCameraController
{
  public:
	EditorCameraController(const desktop::Window& window, const desktop::InputState& inputState);

	void toggleControl(ecs::Registry& registry);

	void onUpdateFrame(ecs::Registry& registry, float delta);

	void onEditorViewResized(ecs::Registry& registry, double aspectRatio);

  private:
	const desktop::Window& mWindow;
	const desktop::InputState& mInputState;
};

} // namespace spatial::editor