#include <spatial/ecs/Camera.h>
#include <spatial/render/Camera.h>
#include <spatial/render/CameraController.h>
#include <spatial/render/Entity.h>

namespace spatial::render
{

CameraController::CameraController(filament::Engine& engine) : mEngine{engine}
{
}

void CameraController::onUpdateFrame(ecs::Registry& registry) const
{
	createCameras(registry);
	updateCameras(registry);
	deleteCameras(registry);
}

void CameraController::deleteCameras(ecs::Registry& registry) const
{
	auto view = registry.template getEntities<Camera>(
		ecs::ExcludeComponents<ecs::OrthographicCamera, ecs::CustomCamera, ecs::PerspectiveCamera>);
	registry.removeComponent<Camera>(view.begin(), view.end());
}

void CameraController::updateCameras(ecs::Registry& registry) const
{
	const auto updateCameraFn = [this](const auto& data, auto& camera) { update(data, camera); };

	registry.getEntities<const ecs::OrthographicCamera, Camera>().each(updateCameraFn);
	registry.getEntities<const ecs::PerspectiveCamera, Camera>().each(updateCameraFn);
	registry.getEntities<const ecs::CustomCamera, Camera>().each(updateCameraFn);
}

void CameraController::createCameras(ecs::Registry& registry) const
{
	const auto createCameraFn = [&, this](ecs::Entity entity, const auto& renderable, const auto& component) {
		auto& camera = registry.addComponent<Camera>(entity, mEngine, renderable.get());
		update(component, camera);
	};

	registry.getEntities<const Entity, const ecs::OrthographicCamera>(ecs::ExcludeComponents<Camera>)
		.each(createCameraFn);
	registry.getEntities<const Entity, const ecs::PerspectiveCamera>(ecs::ExcludeComponents<Camera>)
		.each(createCameraFn);
	registry.getEntities<const Entity, const ecs::CustomCamera>(ecs::ExcludeComponents<Camera>).each(createCameraFn);
}

void CameraController::update(const ecs::PerspectiveCamera& data, Camera& camera) const
{
	camera.setPerspectiveProjection(data.fieldOfView, data.aspectRatio, data.near, data.far);
}

void CameraController::update(const ecs::OrthographicCamera& data, Camera& camera) const
{
	camera.setOrthographicProjection(data.left, data.right, data.bottom, data.top, data.near, data.far);
}

void CameraController::update(const ecs::CustomCamera& data, Camera& camera) const
{
	camera.setCustomProjection(data.projectionMatrix, data.near, data.far);
}

} // namespace spatial::render