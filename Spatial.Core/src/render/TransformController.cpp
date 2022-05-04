#include <spatial/ecs/Relation.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Transform.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

void TransformController::createTransforms(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const Entity, ecs::Transform>(ecs::ExcludeComponents<Transform>)
		.each([&](const auto entity, const auto& renderable, const auto& transform) {
			registry.addComponent<Transform>(entity, engine, renderable.get());
		});
}

void TransformController::updateTransforms(ecs::Registry& registry)
{
	registry.getEntities<ecs::Transform, Transform>().each(
		[](const auto& data, auto& transform) { transform.setMatrix(data.getMatrix()); });
}

} // namespace spatial::render