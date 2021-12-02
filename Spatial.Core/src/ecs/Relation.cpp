#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

void Parent::addChild(Registry& registry, Entity parentEntity, Entity childEntity)
{
	auto& parent = registry.getOrAddComponent<ecs::Parent>(parentEntity);
	parent.childrenCount++;

	auto& child = registry.getOrAddComponent<ecs::Child>(childEntity);
	child.parent = parentEntity;

	if (!registry.isValid(parent.first))
	{
		parent.first = childEntity;
	}
	else if (registry.isValid(parent.last))
	{
		auto& lastChild = registry.getComponent<ecs::Child>(parent.last);
		lastChild.next = childEntity;

		child.previous = parent.last;
	}

	parent.last = childEntity;
}

Entity Parent::createChild(Registry& registry, Entity parentEntity)
{
	auto newChild = registry.createEntity();
	Parent::addChild(registry, parentEntity, newChild);

	return newChild;
}

} // namespace spatial::ecs