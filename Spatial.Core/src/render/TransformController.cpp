#include <spatial/ecs/Relation.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Transform.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

void updateTransform(const ecs::Transform& component, render::Transform& transform)
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

} // namespace spatial::render