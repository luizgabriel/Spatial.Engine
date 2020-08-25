#include <math/mat4.h>
#include <math/vec3.h>
#include <spatial/ecs/CameraSystem.h>
#include <spatial/ecs/Components.h>
#include <spatial/render/Resources.h>
#include <variant>

template <class... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace spatial::ecs
{

CameraSystem::CameraSystem(filament::Engine& engine) : mEngine{engine}
{
}

void CameraSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	const auto& camera = registry.get<ecs::Camera>(entity);
	auto renderable = createEntity(mEngine);

	mCameraComponents.emplace(entity, createCamera(mEngine, renderable.get()));
	registry.emplace<ecs::Renderable>(entity, std::move(renderable));
}

void CameraSystem::onUpdate(const entt::registry& registry)
{
	using namespace filament::math;

	const auto view = registry.view<const ecs::Transform, const ecs::Camera>();
	for (entt::entity entity : view)
	{
		const auto& camera = view.get<const ecs::Camera>(entity);
		auto& filamentCameraComponent = mCameraComponents.at(entity);

		std::visit(
			overloaded{
				[&](const ecs::Camera::Perspective& proj) {
					filamentCameraComponent->setProjection(proj.fieldOfView, proj.aspectRatio, proj.near, proj.far);
				},
				[&](const ecs::Camera::Ortographic& proj) {
					filamentCameraComponent->setProjection(filament::Camera::Projection::ORTHO, -proj.aspectRatio,
														   proj.aspectRatio, -1, 1, proj.near, proj.far);
				},
				[&](const ecs::Camera::Custom& proj) {
					filamentCameraComponent->setCustomProjection(proj.projection, proj.near, proj.far);
				},
			},
			camera.projection);

		const auto& transform = view.get<const ecs::Transform>(entity);
		filamentCameraComponent->lookAt(transform.position, camera.target);
	};

}

} // namespace spatial::ecs