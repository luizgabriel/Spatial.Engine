#include "spatial/ecs/TransformSystem.h"
#include <math/mat4.h>
#include <spatial/ecs/Components.h>

namespace spatial::ecs
{

TransformSystem::TransformSystem(filament::Engine& engine) : m_transformManager{engine.getTransformManager()}
{
}

void TransformSystem::onUpdate(entt::registry& registry)
{
	auto view = registry.view<Transform, Renderable>();
	view.each([this, &view](auto& transform, auto& renderable) {
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