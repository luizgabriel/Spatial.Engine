#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct Relation
{
	size_t childrenCount;
	Entity first;
	Entity last;
	Entity previous;
	Entity next;
	Entity parent;

	Relation()
		: childrenCount{0},
		  first{NullEntity},
		  last{NullEntity},
		  previous{NullEntity},
		  next{NullEntity},
		  parent{NullEntity}
	{
	}

	static void addChild(Registry& registry, Entity parent, Entity child);
};

} // namespace spatial::ecs
