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
	// TODO: All create functions are too similar. Can we merge then?

	createOrthographicCameras(registry);
	updateOrthographicCameras(registry);

	createPerspectiveCameras(registry);
	updatePerspectiveCameras(registry);

	createCustomCameras(registry);
	updateCustomCameras(registry);
}

void CameraSystem::createCustomCameras(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::CustomCamera>(ecs::ExcludeComponents<Camera>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Camera>(entity, mEngine, renderable.get());
	}
}

void CameraSystem::updateCustomCameras(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::CustomCamera, Camera>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::CustomCamera>(entity);
		auto& camera = registry.getComponent<Camera>(entity);

		camera.setCustomProjection(data.projectionMatrix, data.near, data.far);
	}
}

void CameraSystem::createOrthographicCameras(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::OrthographicCamera>(ecs::ExcludeComponents<Camera>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		const auto& data = registry.getComponent<const ecs::OrthographicCamera>(entity);

		registry.addComponent<Camera>(entity, mEngine, renderable.get());
	}
}

void CameraSystem::updateOrthographicCameras(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::OrthographicCamera, Camera>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::OrthographicCamera>(entity);
		auto& camera = registry.getComponent<Camera>(entity);

		camera.setOrtographicProjection(data.left, data.right, data.bottom, data.top, data.near, data.far);
	}
}

void CameraSystem::createPerspectiveCameras(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::PerspectiveCamera>(ecs::ExcludeComponents<Camera>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);
		registry.addComponent<Camera>(entity, mEngine, renderable.get());
	}
}

void CameraSystem::updatePerspectiveCameras(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::PerspectiveCamera, Camera>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::PerspectiveCamera>(entity);
		auto& camera = registry.getComponent<Camera>(entity);

		camera.setPerspectiveProjection(data.fieldOfView, data.aspectRatio, data.near, data.far);
	}
}

} // namespace spatial::render