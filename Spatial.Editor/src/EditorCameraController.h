#pragma once

#include "EditorCamera.h"
#include <spatial/desktop/Window.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/UpdateController.h>
#include <spatial/input/InputState.h>

namespace spatial::editor
{

class EditorCameraController : public ecs::UpdateController<ecs::Transform, EditorCamera>
{
  public:
	EditorCameraController() = default;

	void onUpdateInput(const desktop::InputState& state);

  protected:
	void onUpdateEntity(float delta, ecs::Transform& transform, EditorCamera& editor) const noexcept override;

  private:
	struct InputControls
	{
		math::float3 movementOffset{.0f};
		math::float2 viewOffset{.0f};
	};

	InputControls mControls{};
};

} // namespace spatial::editor