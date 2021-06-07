#include <spatial/ecs/SceneNode.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Transform.h>
#include <spatial/render/TransformSystem.h>

namespace spatial::render
{

TransformSystem::TransformSystem(filament::Engine& engine)
	: mEngine{engine}
{
}

void TransformSystem::synchronize(ecs::Registry& registry)
{
	createTransforms(registry);
	updateTransformsParents(registry);
	updateTransforms(registry);
}

void TransformSystem::createTransforms(ecs::Registry& registry) const
{
	auto view = registry.getEntities<Entity, ecs::Transform>(ecs::ExcludeComponents<Transform>);

	for (auto entity : view)
	{
		const auto& renderable = registry.getComponent<const Entity>(entity);

		registry.addComponent<Transform>(entity, mEngine, renderable.get());
	}
}

void TransformSystem::updateTransformsParents(ecs::Registry& registry)
{
	auto view = registry.getEntities<ecs::SceneNode, ecs::Transform, Transform>();

	for (auto entity : view)
	{
		const auto& node = registry.getComponent<const ecs::SceneNode>(entity);
		const auto& data = registry.getComponent<const ecs::Transform>(entity);
		auto& transform = registry.getComponent<Transform>(entity);

		if (registry.isValid(node.parent)) {
			const auto& parentTransform = registry.getComponent<const Transform>(node.parent);
			transform.setParent(parentTransform);
		}
	}
}

void TransformSystem::updateTransforms(ecs::Registry& registry)
{
	auto view = registry.getEntities<ecs::Transform, Transform>();

	for (auto entity : view)
	{
		const auto& data = registry.getComponent<const ecs::Transform>(entity);
		auto& transform = registry.getComponent<Transform>(entity);

		transform.setMatrix(data.getMatrix());
	}
}


} // namespace spatial::render