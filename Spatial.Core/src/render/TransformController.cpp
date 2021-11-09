#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Transform.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

TransformController::TransformController(filament::Engine& engine)
	: mEngine{engine}
{
}

void TransformController::onUpdateFrame(ecs::Registry& registry) const
{
	createTransforms(registry);
	updateTransformsParents(registry);
	updateTransforms(registry);
	//clearRemovedTransforms(registry);
}

void TransformController::createTransforms(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::Transform>(ecs::ExcludeComponents<Transform>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);

		registry.addComponent<Transform>(entity, mEngine, renderable.get());
	}
}

void TransformController::updateTransformsParents(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Transform, Transform>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Transform>(entity);
		auto& transform = registry.getComponent<Transform>(entity);

		// TODO: This branching can be optimized (sorting may help?)
		if (registry.hasAllComponents<ecs::Relation>(entity)) {
			const auto& node = registry.getComponent<const ecs::Relation>(entity);
			if (registry.isValid(node.parent) && registry.hasAllComponents<Transform>(node.parent)) {
				const auto& parentTransform = registry.getComponent<const Transform>(node.parent);
				transform.setParent(parentTransform);
			}
		}
	}
}

void TransformController::updateTransforms(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::Transform, Transform>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Transform>(entity);
		auto& transform = registry.getComponent<Transform>(entity);

		transform.setMatrix(data.getMatrix());
	}
}

void TransformController::clearRemovedTransforms(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Transform>(ecs::ExcludeComponents<ecs::Transform>);

	for (auto entity : view)
	{
		registry.removeComponent<Transform>(entity);
	}
}

} // namespace spatial::render