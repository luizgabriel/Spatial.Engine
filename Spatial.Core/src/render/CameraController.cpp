#include <spatial/ecs/Camera.h>
#include <spatial/render/Camera.h>
#include <spatial/render/CameraController.h>
#include <spatial/render/Entity.h>

namespace spatial::render
{

void CameraController::deleteCameras(ecs::Registry& registry)
{
	auto view = registry.getEntities<Camera>(ecs::ExcludeComponents<ecs::OrthographicCamera, ecs::CustomCamera, ecs::PerspectiveCamera>);
	registry.removeComponent<Camera>(view.begin(), view.end());
}

void CameraController::updateCameras(ecs::Registry& registry)
{
	registry.getEntities<const ecs::OrthographicCamera, Camera>().each([](const auto& data, auto& camera) {
		camera.setOrthographicProjection(data.left, data.right, data.bottom, data.top, data.near, data.far);
	});

	registry.getEntities<const ecs::PerspectiveCamera, Camera>().each([](const auto& data, auto& camera) {
		camera.setPerspectiveProjection(data.fieldOfView, data.aspectRatio, data.near, data.far);
	});

	registry.getEntities<const ecs::CustomCamera, Camera>().each(
		[](const auto& data, auto& camera) { camera.setCustomProjection(data.projectionMatrix, data.near, data.far); });
}

void CameraController::createCameras(filament::Engine& engine, ecs::Registry& registry)
{
	const auto createCameraFn = [&](ecs::Entity entity, const auto& renderable, const auto& component) {
		registry.addComponent<Camera>(entity, engine, renderable.get());
	};

	registry.getEntities<const Entity, const ecs::OrthographicCamera>(ecs::ExcludeComponents<Camera>)
		.each(createCameraFn);
	registry.getEntities<const Entity, const ecs::PerspectiveCamera>(ecs::ExcludeComponents<Camera>)
		.each(createCameraFn);
	registry.getEntities<const Entity, const ecs::CustomCamera>(ecs::ExcludeComponents<Camera>).each(createCameraFn);
}

} // namespace spatial::render