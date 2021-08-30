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
	EditorCameraController(const desktop::Window& window);

	void toggleControl(ecs::Registry& registry);

	void onUpdateFrame(ecs::Registry& registry, float delta);

	void onEditorViewResized(ecs::Registry& registry, double aspectRatio);

	void onUpdateInput(const desktop::InputState& state);

  private:
	struct InputControls {
		math::float3 movementOffset{.0f};
		math::float2 viewOffset{.0f};
		bool cancelled{false};
		bool started{false};
	};

	const desktop::Window& mWindow;
	InputControls mControls;
};

} // namespace spatial::editor