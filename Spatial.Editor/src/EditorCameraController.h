#pragma once

#include "EditorCamera.h"
#include <spatial/desktop/InputState.h>
#include <spatial/desktop/Window.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/UpdateSystem.h>

namespace spatial::editor
{

class EditorCameraController : public ecs::UpdateSystem<ecs::Transform, EditorCamera>
{
  public:
	using Base = ecs::UpdateSystem<ecs::Transform, EditorCamera>;

	EditorCameraController() = default;

	bool toggleControl() noexcept;

	void disable() noexcept;

	void onEditorViewResized(ecs::Registry& registry, double aspectRatio);

	void onUpdateInput(const desktop::InputState& state);

  protected:
	void beforeUpdateEntities(ecs::Registry& registry) override;

	void onUpdateEntity(float delta, ecs::Transform& transform, EditorCamera& editor) const noexcept override;

  private:
	struct InputControls
	{
		math::float3 movementOffset{.0f};
		math::float2 viewOffset{.0f};
	};

	InputControls mControls{};
	bool mEnabled{false};
	int mJustStarted{0};

	[[nodiscard]] bool hasEnabledControls() const noexcept
	{
		return mEnabled && mJustStarted >= 10;
	};
};

} // namespace spatial::editor