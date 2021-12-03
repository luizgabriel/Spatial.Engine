#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Relation.h>
#include <vector>
#include <deque>

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

void Child::destroy(Registry& registry, Entity childEntity)
{
	//TODO: As each child work as a linked list. When deleting, we need to update the siblings and the parent.
}

void Parent::destroyChildren(Registry& registry, Entity parentEntity)
{
	//TODO: We need a way to remove all children from the parent
}

std::vector<Entity> Parent::getChildren(const Registry& registry, Entity parentEntity)
{
	const auto* parent = registry.tryGetComponent<const ecs::Parent>(parentEntity);
	if (!parent || parent->childrenCount == 0) return {};

	auto children = std::vector<Entity>(parent->childrenCount);

	Parent::forEachChild(registry, parentEntity, [&](auto entity){
		children.template emplace_back(entity);
	});

	return children;
}


} // namespace spatial::ecs