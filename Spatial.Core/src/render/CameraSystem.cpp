#include <spatial/ecs/Camera.h>
#include <spatial/render/Camera.h>
#include <spatial/render/CameraSystem.h>
#include <spatial/render/Entity.h>

namespace spatial::render
{

CameraSystem::CameraSystem(filament::Engine& engine) : mEngine{engine}
{
}

void CameraSystem::synchronize(ecs::Registry& registry) const
{
	createCameras<ecs::OrthographicCamera>(registry);
	updateCameras<ecs::OrthographicCamera>(registry);

	createCameras<ecs::PerspectiveCamera>(registry);
	updateCameras<ecs::PerspectiveCamera>(registry);

	createCameras<ecs::CustomCamera>(registry);
	updateCameras<ecs::CustomCamera>(registry);

	clearRemovedCameras<ecs::OrthographicCamera, ecs::CustomCamera, ecs::PerspectiveCamera>(registry);
}

void CameraSystem::update(const ecs::PerspectiveCamera& data, Camera& camera) const
{
	camera.setPerspectiveProjection(data.fieldOfView, data.aspectRatio, data.near, data.far);
}

void CameraSystem::update(const ecs::OrthographicCamera& data, Camera& camera) const
{
	camera.setOrtographicProjection(data.left, data.right, data.bottom, data.top, data.near, data.far);
}

void CameraSystem::update(const ecs::CustomCamera& data, Camera& camera) const
{
	camera.setCustomProjection(data.projectionMatrix, data.near, data.far);
}

} // namespace spatial::render