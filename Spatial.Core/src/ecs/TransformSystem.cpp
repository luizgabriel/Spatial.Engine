#include "spatial/ecs/TransformSystem.h"
#include <spatial/ecs/Components.h>
#include <math/mat4.h>
#ifndef SPATIAL_PLATFORM_OSX
#include <execution>
#endif

namespace spatial::ecs
{

TransformSystem::TransformSystem(filament::Engine& engine) : m_transformManager{engine.getTransformManager()}
{
}

void TransformSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<Transform, Renderable>();
#ifndef SPATIAL_PLATFORM_OSX
	std::for_each(std::execution::par_unseq, view.begin(), view.end(), [this, &view](auto entity) {
		auto& [transform, renderable] = view.get<Transform, Renderable>(entity);
#else
	view.each([this, &view](auto& transform, auto& renderable) {
#endif
		auto transformEntity = m_transformManager.getInstance(renderable.entity.get());
		auto model = m_transformManager.getTransform(transformEntity);
		model[3].xyz = transform.position;
		model[0].x = transform.scale.x;
		model[1].y = transform.scale.y;
		model[2].z = transform.scale.z;

		m_transformManager.setTransform(transformEntity, model);
	});
}

} // namespace spatial::ecs