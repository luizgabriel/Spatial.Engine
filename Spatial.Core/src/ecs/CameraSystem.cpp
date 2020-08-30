#include <math/mat4.h>
#include <math/vec3.h>
#include <spatial/ecs/CameraSystem.h>
#include <spatial/ecs/Components.h>
#include <spatial/render/Resources.h>

namespace spatial::ecs
{

CameraSystem::CameraSystem(filament::Engine& engine) : mEngine{engine}
{
}

void CameraSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	auto& renderable = registry.get_or_emplace<ecs::SceneEntity>(entity);
	auto component = createCamera(mEngine, renderable.entity);

	if (auto camera = registry.try_get<ecs::PerspectiveCamera>(entity); camera) {
		component->setProjection(camera->fieldOfView, camera->aspectRatio, camera->near, camera->far);
	}

	if (auto camera = registry.try_get<ecs::OrtographicCamera>(entity); camera) {
		component->setProjection(fl::Camera::Projection::ORTHO, -camera->aspectRatio, camera->aspectRatio, -1, 1, camera->near, camera->far);
	}

	if (auto camera = registry.try_get<ecs::CustomCamera>(entity); camera) {
		component->setCustomProjection(camera->projection, camera->near, camera->far);
	}

	mCameraComponents.emplace(entity, createCamera(mEngine, renderable.entity));
	registry.emplace<ecs::SceneEntity>(entity, std::move(renderable));
}

void CameraSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	mCameraComponents.erase(entity);
}

void CameraSystem::onUpdate(const entt::registry& registry)
{
	const auto pView = registry.view<const ecs::PerspectiveCamera>();
	for (entt::entity entity : pView)
	{
		const auto& camera = pView.get<const ecs::PerspectiveCamera>(entity);
		auto& component = mCameraComponents.at(entity);

		component->setProjection(camera.fieldOfView, camera.aspectRatio, camera.near, camera.far);
	}

	const auto oView = registry.view<const ecs::OrtographicCamera>();
	for (entt::entity entity : oView)
	{
		const auto& camera = oView.get<const ecs::OrtographicCamera>(entity);
		auto& component = mCameraComponents.at(entity);

		component->setProjection(fl::Camera::Projection::ORTHO, -camera.aspectRatio, camera.aspectRatio, -1, 1, camera.near, camera.far);
	}

	const auto cView = registry.view<const ecs::CustomCamera>();
	for (entt::entity entity : cView)
	{
		const auto& camera = cView.get<const ecs::CustomCamera>(entity);
		auto& component = mCameraComponents.at(entity);

		component->setCustomProjection(camera.projection, camera.near, camera.far);
	}

	const auto tView = registry.view<const ecs::Transform, const ecs::CameraTarget>();
	for (entt::entity entity : tView)
	{
		const auto& target = tView.get<const ecs::CameraTarget>(entity);
		const auto& transform = tView.get<const ecs::Transform>(entity);
		auto& component = mCameraComponents.at(entity);

		component->lookAt(transform.position, transform.position + target.target);
	}
}


} // namespace spatial::ecs