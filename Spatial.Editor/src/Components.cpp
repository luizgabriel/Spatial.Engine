#include "Components.h"
#include <spatial/ecs/View.h>

namespace spatial::editor
{

void tags::IsSelected::replace(ecs::Registry& registry, ecs::Entity entity)
{
	registry.removeComponentFromEntities<IsSelected>();
	registry.addComponent<IsSelected>(entity);
}

void EditorCamera::replaceCamera(ecs::Registry& registry, ecs::Entity newControlledEntity)
{
	registry.removeComponentFromEntities<editor::EditorCamera>();
	registry.addComponent<editor::EditorCamera>(newControlledEntity);
}

void EditorCamera::replaceView(ecs::Registry& registry, ecs::Entity newControlledView)
{
	if (!registry.hasComponent<ecs::View>(newControlledView))
		return;

	registry.removeComponentFromEntities<editor::tags::IsSelectedView>();
	registry.addComponent<editor::tags::IsSelectedView>(newControlledView);

	auto& sceneView = registry.getComponent<ecs::View>(newControlledView);
	replaceCamera(registry, sceneView.camera);
}

} // namespace spatial::editor
