#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Relation.h>

namespace spatial::ecs
{

void Relation::addChild(Registry& registry, Entity parent, Entity child)
{
	auto& parentRelation = registry.getComponent<ecs::Relation>(parent);
	parentRelation.childrenCount++;

	auto& childRelation = registry.getComponent<ecs::Relation>(child);
	childRelation.parent = parent;

	if (!registry.isValid(parentRelation.first))
	{
		parentRelation.first = child;
	}
	else if (registry.isValid(parentRelation.last))
	{
		auto& lastChildRelation = registry.getComponent<ecs::Relation>(parentRelation.last);
		lastChildRelation.next = child;

		childRelation.previous = parentRelation.last;
	}

	parentRelation.last = child;
}

} // namespace spatial::ecs