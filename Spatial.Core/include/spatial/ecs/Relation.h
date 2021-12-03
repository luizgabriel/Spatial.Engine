#pragma once

#include <spatial/ecs/Registry.h>
#include <type_traits>

namespace spatial::ecs
{

struct Child
{
	Entity parent;
	Entity previous;
	Entity next;

	Child() : parent{NullEntity}, previous{NullEntity}, next{NullEntity}
	{
	}

	static void destroy(Registry& registry, Entity childEntity);
};

struct Parent
{
	size_t childrenCount;
	Entity first;
	Entity last;

	Parent() : childrenCount{0}, first{NullEntity}, last{NullEntity}
	{
	}

	static Entity createChild(Registry& registry, Entity parentEntity);

	static void addChild(Registry& registry, Entity parentEntity, Entity childEntity);

	/**
	 * We need a way
	 * @param registry
	 * @param parentEntity
	 */
	static void destroyChildren(Registry& registry, Entity parentEntity);

	static std::vector<Entity> getChildren(const Registry& registry, Entity parentEntity);

	template <typename Function>
	static void forEachChild(const Registry& registry, Entity parentEntity, Function each)
	{
		static_assert(std::is_invocable_v<Function, Entity, const Child&> || std::is_invocable_v<Function, Entity>, "The function argument needs to be invocable with `each(Entity)` or `each(Entity, const Child&)`");

		const auto* parent = registry.template tryGetComponent<const Parent>(parentEntity);
		if (!parent || parent->childrenCount == 0) return;

		auto currentChildEntity = parent->first;
		for (auto i = size_t{0}; i < parent->childrenCount; i++)
		{
			const auto& child = registry.getComponent<const Child>(currentChildEntity);

			if constexpr (std::is_invocable_v<Function, Entity, const Child&>)
				each(currentChildEntity, child);
			else if constexpr(std::is_invocable_v<Function, Entity>)
				each(currentChildEntity);

			currentChildEntity = child.next;
		}
	}
};

} // namespace spatial::ecs
