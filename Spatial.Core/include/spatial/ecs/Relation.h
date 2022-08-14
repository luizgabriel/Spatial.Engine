#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct Child
{
	constexpr static auto typeName = "child";

	Entity parent;
	Entity previous;
	Entity next;

	Child() : parent{NullEntity}, previous{NullEntity}, next{NullEntity}
	{
	}

	/**
	 * @description Detaches a childEntity from the parent
	 */
	static void remove(Registry& registry, Entity childEntity);

	/**
	 * @description Moves some entity from it current parent to a new one
	 */
	static void changeParent(Registry& registry, entt::entity childEntity, Entity newParentEntity);
};

struct Parent
{
	constexpr static auto typeName = "parent";

	size_t childrenCount;
	Entity first;
	Entity last;

	Parent() : childrenCount{0}, first{NullEntity}, last{NullEntity}
	{
	}

	/**
	 * @description Creates a new entity and adds as a child of the parentEntity
	 * @return The newly created entity
	 */
	static Entity createChild(Registry& registry, Entity parentEntity);

	/**
	 * @description Add childEntity as Child of parentEntity by adding the Child Component and updating the Parent
	 * attributes
	 */
	static void addChild(Registry& registry, Entity parentEntity, Entity childEntity);

	/**
	 * @description Returns true if the targetEntity is child of parentEntity in a Depth-First-Search
	 */
	static bool isChild(const Registry& registry, Entity parentEntity, Entity targetEntity);

	/**
	 * @description Detaches all children and removes the ecs::Parent component
	 */
	static void remove(Registry& registry, Entity parentEntity);

	/**
	 * @description Removes the Parent component and Destroys every single child of the parentEntity even if any child
	 * is also parent of other children.
	 * Time Complexity: O(n), n is the total number of children
	 * @returns The number of destroyed entities
	 */
	static size_t destroyChildren(Registry& registry, Entity parentEntity);

	/**
	 * @descriptions Collects a set of child entities
	 */
	static std::vector<Entity> getChildren(const Registry& registry, Entity parentEntity);

	/**
	 * @descriptions Collects a vector of child entities recursively
	 * Time Complexity: O(n), n is the total number of children
	 */
	static std::vector<Entity> getChildrenDepth(const Registry& registry, Entity parentEntity);

	/**
	 * @description Visits every direct child of the parentEntity
	 * Time Complexity: O(c), c is the direct number of children
	 */
	template <typename Function>
	static void forEachChild(const Registry& registry, Entity parentEntity, Function visitor)
	{
		static_assert(
			std::is_invocable_v<Function, Entity, const Child&> || std::is_invocable_v<Function, Entity>,
			"The visitor needs to be invocable with `visitor(Entity)` or `visitor(Entity, const Child&)`");

		if (!registry.hasComponent<ecs::Parent>(parentEntity))
			return;

		const auto& parent = registry.getComponent<const Parent>(parentEntity);
		const auto childrenCount = parent.childrenCount;
		auto currentChildEntity = parent.first;

		for (auto i = size_t{0}; i < childrenCount; i++)
		{
			const auto& child = registry.getComponent<const Child>(currentChildEntity);
			const auto nextEntity = child.next;

			if constexpr (std::is_invocable_v<Function, Entity, const Child&>)
				visitor(currentChildEntity, child);
			else if constexpr (std::is_invocable_v<Function, Entity>)
				visitor(currentChildEntity);

			currentChildEntity = nextEntity;
		}
	}

	/**
	 * @description If returns the number of children for some Parent.
	 * Time Complexity: O(1)
	 */
	static size_t getChildrenCount(const Registry& registry, ecs::Entity entity);
};

} // namespace spatial::ecs
