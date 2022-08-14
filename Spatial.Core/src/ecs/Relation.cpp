#include <deque>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Relation.h>
#include <vector>

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

std::vector<Entity> Parent::getChildren(const Registry& registry, Entity parentEntity)
{
	if (!registry.hasComponent<ecs::Parent>(parentEntity))
		return {};

	const auto& parent = registry.getComponent<const Parent>(parentEntity);
	auto children = std::vector<Entity>{};
	children.reserve(parent.childrenCount);

	Parent::forEachChild(registry, parentEntity, [&](auto entity) { children.emplace_back(entity); });

	return children;
}

std::vector<Entity> Parent::getChildrenDepth(const Registry& registry, Entity parentEntity)
{
	if (!registry.hasComponent<ecs::Parent>(parentEntity))
		return {};

	const auto& parent = registry.getComponent<const Parent>(parentEntity);
	auto children = std::vector<Entity>{};
	children.reserve(parent.childrenCount);

	auto stack = std::deque<Entity>{parentEntity};

	while (!stack.empty())
	{
		auto currentEntity = stack.back();
		stack.pop_back();

		if (registry.hasAnyComponent<Parent>(currentEntity))
			Parent::forEachChild(registry, currentEntity, [&](auto child) { stack.push_front(child); });

		if (currentEntity != parentEntity)
			children.emplace_back(currentEntity);
	}

	return children;
}

Entity Parent::createChild(Registry& registry, Entity parentEntity)
{
	auto newChild = registry.createEntity();
	Parent::addChild(registry, parentEntity, newChild);

	return newChild;
}

void Child::remove(Registry& registry, Entity childEntity)
{
	if (!registry.hasComponent<ecs::Child>(childEntity))
		return;

	const auto& child = registry.getComponent<const Child>(childEntity);
	auto& parent = registry.getComponent<Parent>(child.parent);
	parent.childrenCount--;

	auto* previousChild = registry.tryGetComponent<Child>(child.previous);
	auto* nextChild = registry.tryGetComponent<Child>(child.next);

	if (previousChild)
		previousChild->next = child.next;

	if (nextChild)
		nextChild->previous = child.previous;

	if (parent.first == childEntity)
		parent.first = child.next;

	if (parent.last == childEntity)
		parent.last = child.previous;

	registry.removeComponent<Child>(childEntity);
}

void Child::changeParent(Registry& registry, entt::entity childEntity, Entity newParentEntity)
{
	assert(childEntity != newParentEntity);
	Child::remove(registry, childEntity);
	Parent::addChild(registry, newParentEntity, childEntity);
}

void Parent::remove(Registry& registry, Entity parentEntity)
{
	Parent::forEachChild(registry, parentEntity, [&](auto child) { registry.removeComponent<Child>(child); });

	registry.removeComponent<Parent>(parentEntity);
}

bool Parent::isChild(const Registry& registry, Entity parentEntity, Entity targetEntity)
{
	auto stack = std::deque<Entity>{parentEntity};

	while (!stack.empty())
	{
		auto currentEntity = stack.back();
		stack.pop_back();

		if (currentEntity == targetEntity && parentEntity != targetEntity)
			return true;

		if (registry.hasAnyComponent<Parent>(currentEntity))
			Parent::forEachChild(registry, currentEntity, [&](auto child) { stack.push_front(child); });
	}

	return false;
}

size_t Parent::destroyChildren(Registry& registry, Entity parentEntity)
{
	size_t destroyed = 0;
	auto stack = std::deque<Entity>{parentEntity};

	while (!stack.empty())
	{
		auto currentEntity = stack.back();
		stack.pop_back();

		if (registry.hasAnyComponent<Parent>(currentEntity))
			Parent::forEachChild(registry, currentEntity, [&](auto child) { stack.push_front(child); });

		if (currentEntity != parentEntity)
		{
			registry.destroyEntity(currentEntity);
			destroyed++;
		}
	}

	registry.removeComponent<Parent>(parentEntity);
	return destroyed;
}

size_t Parent::getChildrenCount(const Registry& registry, ecs::Entity entity)
{
	if (!registry.hasComponent<Parent>(entity))
		return 0;

	const auto& parent = registry.getComponent<Parent>(entity);
	return parent.childrenCount;
}

} // namespace spatial::ecs