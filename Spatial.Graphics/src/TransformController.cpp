#include <spatial/ecs/Relation.h>
#include <spatial/graphics/Entity.h>
#include <spatial/graphics/Transform.h>
#include <spatial/graphics/TransformController.h>

namespace spatial::graphics
{

void updateTransform(const ecs::Transform& component, graphics::Transform& transform)
{
	transform.setMatrix(component.getMatrix());
}

void TransformController::createTransforms(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const Entity, ecs::Transform>(ecs::ExcludeComponents<Transform>)
		.each([&](const auto entity, const auto& renderable, const auto& component) {
			auto& transform = registry.addComponent<Transform>(entity, engine, renderable.get());
			updateTransform(component, transform);
		});
}

void TransformController::updateTransforms(ecs::Registry& registry)
{
	registry.getEntities<ecs::Transform, Transform>().each(updateTransform);
}

} // namespace spatial::graphics