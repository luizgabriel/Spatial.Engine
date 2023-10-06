#include "DragAndDrop.h"
#include "../ecs/Components.h"
#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/Builder.h>
#include <spatial/ui/components/DragAndDrop.h>

namespace spatial::ui
{

bool EditorDragAndDrop::loadScene(std::string& scenePath)
{
	auto dnd = DragAndDropTarget{};
	const auto result = dnd.getPayload();
	if (result && boost::algorithm::ends_with(result->c_str(), ".spatial.json"))
	{
		scenePath = std::string{result.value()};
		return true;
	}

	return false;
}

void EditorDragAndDrop::loadMeshInstance(ecs::Registry& registry, math::vec3 createEntityPosition)
{
	auto dnd = DragAndDropTarget{};
	auto result = dnd.getPayload();
	ecs::Entity createdEntity = ecs::NullEntity;

	if (result && boost::algorithm::ends_with(result->c_str(), ".filamesh"))
	{
		createdEntity = ecs::Builder::create(registry)
							.asTransform()
							.withPosition(createEntityPosition)
							.asMeshInstance()
							.withMesh(*result)
							.withShadowOptions(true, true);
	}

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

} // namespace spatial::ui