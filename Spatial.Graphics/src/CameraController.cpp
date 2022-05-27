#include <spatial/ecs/Camera.h>
#include <spatial/graphics/Camera.h>
#include <spatial/graphics/CameraController.h>
#include <spatial/graphics/Entity.h>
#include <spatial/graphics/Resources.h>

namespace spatial::graphics
{

void CameraController::deleteCameras(ecs::Registry& registry)
{
	auto view = registry.getEntities<SharedCamera>(
		ecs::ExcludeComponents<ecs::OrthographicCamera, ecs::CustomCamera, ecs::PerspectiveCamera>);
	registry.removeComponent<SharedCamera>(view.begin(), view.end());
}

void updateCamera(const ecs::OrthographicCamera& component, SharedCamera& camera)
{
	camera->setOrthographicProjection(component.left, component.right, component.bottom, component.top, component.near,
									  component.far);
}

void updateCamera(const ecs::PerspectiveCamera& component, SharedCamera& camera)
{
	camera->setPerspectiveProjection(component.fieldOfView, component.aspectRatio, component.near, component.far);
}

void updateCamera(const ecs::CustomCamera& component, SharedCamera& camera)
{
	camera->setCustomProjection(component.projectionMatrix, component.near, component.far);
}

void CameraController::updateCameras(ecs::Registry& registry)
{
	registry.getEntities<const ecs::OrthographicCamera, SharedCamera>().each(
		[](const ecs::OrthographicCamera& component, SharedCamera& camera) { updateCamera(component, camera); });

	registry.getEntities<const ecs::PerspectiveCamera, SharedCamera>().each(
		[](const ecs::PerspectiveCamera& component, SharedCamera& camera) { updateCamera(component, camera); });

	registry.getEntities<const ecs::CustomCamera, SharedCamera>().each(
		[](const ecs::CustomCamera& component, SharedCamera& camera) { updateCamera(component, camera); });
}

void CameraController::createCameras(filament::Engine& engine, ecs::Registry& registry)
{
	const auto createCameraFn = [&](ecs::Entity entity, const auto& renderable, const auto& component) {
		auto& camera = registry.addComponent(entity, std::make_shared<Camera>(createCamera(engine, renderable.get())));
		updateCamera(component, camera);
	};

	registry.getEntities<const Entity, const ecs::OrthographicCamera>(ecs::ExcludeComponents<SharedCamera>)
		.each(createCameraFn);
	registry.getEntities<const Entity, const ecs::PerspectiveCamera>(ecs::ExcludeComponents<SharedCamera>)
		.each(createCameraFn);
	registry.getEntities<const Entity, const ecs::CustomCamera>(ecs::ExcludeComponents<SharedCamera>)
		.each(createCameraFn);
}

} // namespace spatial::graphics