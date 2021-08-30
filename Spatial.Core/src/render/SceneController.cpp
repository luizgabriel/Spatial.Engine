#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SceneController.h>

namespace spatial::render
{

SceneController::SceneController(filament::Engine& engine, filament::Scene& scene)
	: mEngine{engine}, mScene{scene}
{
}

void SceneController::onUpdateFrame(ecs::Registry& registry) const
{
	auto view = registry.getEntities<ecs::tags::IsRenderable>(ecs::ExcludeComponents<Entity>);

	for (auto entity : view)
	{
		const auto& renderable = registry.addComponent<Entity>(entity, createEntity(mEngine));
		mScene.addEntity(renderable.get());
	}
}

} // namespace spatial::render