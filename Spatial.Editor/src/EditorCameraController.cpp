#include "EditorCameraController.h"
#include "EditorCamera.h"
#include "Tags.h"
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ui/components/Components.h>

using namespace spatial::math;

namespace spatial::editor
{

void createCamera(ecs::Registry& registry)
{
	ecs::build(registry)
		.withName("Editor Camera")
		.with(EditorCamera{.5f, 10.0f})
		.with<tags::IsEditorEntity>()
		.asTransform()
		.withPosition({3.0f, 3.0f, 20.0f})
		.asPerspectiveCamera()
		.withFieldOfView(60.0)
		.withAspectRatio(19.0 / 6.0);
}

void EditorCameraController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	if (!registry.hasAnyEntity<EditorCamera>())
		createCamera(registry);

	registry.getEntities<ecs::Transform, EditorCamera>().each([&](auto& transform, auto& options) {
		const auto movementDelta = delta * options.velocity * mControls.movementOffset;
		const auto forward = math::forwardVector(transform.getMatrix());
		const auto right = cross(forward, math::axisY);

		transform.position += forward * movementDelta.x;
		transform.position += right * movementDelta.y;
		transform.position += math::axisY * movementDelta.z;

		if (mControls.cancelled)
		{
			options.enabled = false;
			options.justStarted = 0;
		}

		if (mControls.started || options.startPressed)
		{
			mWindow.warpMouse(mWindow.getSize() * .5f);
			options.enabled = true;
			options.startPressed = false;
			options.justStarted = 1;
		}

		if (options.enabled && options.justStarted <= 10)
			options.justStarted++;

		if (options.enabled && options.justStarted >= 10)
		{
			const auto viewRotation = mControls.viewOffset * math::pi * options.sensitivity * delta;
			transform.rotation = math::float3{
				std::clamp(transform.rotation.x + viewRotation.y, -math::pi / 2.0f,
						   math::pi / 2.0f), // clamp the pitch
				transform.rotation.y + viewRotation.x,
				.0f // remove the roll
			};
		}
	});
}

EditorCameraController::EditorCameraController(const desktop::Window& window) : mWindow{window}
{
}

void EditorCameraController::onEditorViewResized(ecs::Registry& registry, double aspectRatio)
{
	registry.getEntities<ecs::PerspectiveCamera, EditorCamera>().each(
		[=](auto& camera, auto&) { camera.aspectRatio = aspectRatio; });

	registry.getEntities<ecs::OrthographicCamera, EditorCamera>().each([=](auto& camera, auto&) {
		camera.left = -aspectRatio;
		camera.right = aspectRatio;
	});
}

void EditorCameraController::toggleControl(ecs::Registry& registry)
{
	registry.getEntities<EditorCamera>().each([](auto& options) {
		if (options.enabled)
		{
			options.justStarted = 0;
			options.enabled = false;
		}
		else
		{
			options.startPressed = true;
		}
	});
}

void EditorCameraController::onUpdateInput(const desktop::InputState& input)
{
	mControls = {
		{
			input.axis(Key::W, Key::S),
			input.axis(Key::D, Key::A),
			input.axis(Key::Space, Key::LShift),
		},
		input.getMouseOffset(),
		input.released(Key::Escape),
		input.combined(Key::LControl, Key::MouseLeft),
	};
}

} // namespace spatial::editor