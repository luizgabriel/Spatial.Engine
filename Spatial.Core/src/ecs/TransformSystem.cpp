#include "spatial/ecs/TransformSystem.h"
#include <spatial/ecs/Components.h>
#include <execution>

namespace spatial::ecs
{

TransformSystem::TransformSystem(filament::Engine& engine) : m_transformManager{engine.getTransformManager()}
{
}

void TransformSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<Transform, Renderable>();

	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [this, &registry](auto entity) {
		auto& transform = registry.get<Transform>(entity);
		auto& renderable = registry.get<Renderable>(entity);

		auto transformEntity = m_transformManager.getInstance(renderable.entity);
		auto model = m_transformManager.getTransform(transformEntity);
		model[3].xyz = transform.position;
		model[0].x = transform.scale.x;
		model[1].y = transform.scale.y;
		model[2].z = transform.scale.z;

		m_transformManager.setTransform(transformEntity, model);
	});
}

} // namespace spatial::ecs