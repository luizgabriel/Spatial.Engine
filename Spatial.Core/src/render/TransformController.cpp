#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Transform.h>
#include <spatial/render/TransformController.h>

namespace spatial::render
{

TransformController::TransformController(filament::Engine& engine) : mEngine{engine}
{
}

void TransformController::onUpdateFrame(ecs::Registry& registry) const
{
	createTransforms(registry);
	updateTransforms(registry);
	clearRemovedTransforms(registry);
}

void TransformController::createTransforms(ecs::Registry& registry) const
{
	auto view = registry.getEntities<const Entity, ecs::Transform>(ecs::ExcludeComponents<Transform>);
	view.each([&](const auto entity, const auto& renderable, const auto& transform) {
		registry.addComponent<Transform>(entity, mEngine, renderable.get());
	});
}

void TransformController::updateTransforms(ecs::Registry& registry)
{
	auto view = registry.getEntities<ecs::Transform, Transform>();
	view.each([](const auto& data, auto& transform) { transform.setMatrix(data.getMatrix()); });
}

void TransformController::clearRemovedTransforms(ecs::Registry& registry)
{
	auto view = registry.getEntities<Transform>(ecs::ExcludeComponents<ecs::Transform>);
	registry.removeComponent<Transform>(view.begin(), view.end());
}

} // namespace spatial::render