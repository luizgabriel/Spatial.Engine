#include <gtest/gtest.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Relation.h>

using namespace spatial;

TEST(RegistryRelation, NoChild)
{
	auto registry = ecs::Registry{};

	auto entity = registry.createEntity();
	auto& parent = registry.getOrAddComponent<ecs::Parent>(entity);

	ASSERT_EQ(parent.childrenCount, 0);
	ASSERT_FALSE(registry.isValid(parent.first));
	ASSERT_FALSE(registry.isValid(parent.last));
}

TEST(RegistryRelation, OneChild)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	auto childEntity = registry.createEntity();

	ecs::Parent::addChild(registry, parentEntity, childEntity);

	const auto& parent = registry.getComponent<const ecs::Parent>(parentEntity);
	const auto& child = registry.getComponent<const ecs::Child>(childEntity);

	ASSERT_EQ(parent.childrenCount, 1);
	ASSERT_TRUE(registry.isValid(parent.first));
	ASSERT_TRUE(registry.isValid(parent.last));
	ASSERT_EQ(parent.first, parent.last);

	ASSERT_EQ(child.parent, parentEntity);
	ASSERT_FALSE(registry.isValid(child.previous));
	ASSERT_FALSE(registry.isValid(child.next));
}

TEST(RegistryRelation, TwoChildren)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	auto child1Entity = registry.createEntity();
	auto child2Entity = registry.createEntity();
	ecs::Parent::addChild(registry, parentEntity, child1Entity);
	ecs::Parent::addChild(registry, parentEntity, child2Entity);

	const auto& parent = registry.getComponent<const ecs::Parent>(parentEntity);
	const auto& child1 = registry.getComponent<const ecs::Child>(child1Entity);
	const auto& child2 = registry.getComponent<const ecs::Child>(child2Entity);

	ASSERT_EQ(parent.childrenCount, 2);
	ASSERT_TRUE(registry.isValid(parent.first));
	ASSERT_TRUE(registry.isValid(parent.last));
	ASSERT_EQ(parent.first, child1Entity);
	ASSERT_EQ(parent.last, child2Entity);

	ASSERT_EQ(child1.parent, parentEntity);
	ASSERT_EQ(child2.parent, parentEntity);

	ASSERT_FALSE(registry.isValid(child1.previous));
	ASSERT_FALSE(registry.isValid(child2.next));
	ASSERT_EQ(child1.next, child2Entity);
	ASSERT_EQ(child2.previous, child1Entity);
}

TEST(RegistryRelation, ThreeChildren)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	auto child1Entity = registry.createEntity();
	auto child2Entity = registry.createEntity();
	auto child3Entity = registry.createEntity();
	ecs::Parent::addChild(registry, parentEntity, child1Entity);
	ecs::Parent::addChild(registry, parentEntity, child2Entity);
	ecs::Parent::addChild(registry, parentEntity, child3Entity);

	const auto& parent = registry.getComponent<const ecs::Parent>(parentEntity);
	const auto& child1 = registry.getComponent<const ecs::Child>(child1Entity);
	const auto& child2 = registry.getComponent<const ecs::Child>(child2Entity);
	const auto& child3 = registry.getComponent<const ecs::Child>(child3Entity);

	ASSERT_EQ(parent.childrenCount, 3);
	ASSERT_TRUE(registry.isValid(parent.first));
	ASSERT_TRUE(registry.isValid(parent.last));
	ASSERT_EQ(parent.first, child1Entity);
	ASSERT_EQ(parent.last, child3Entity);

	ASSERT_EQ(child1.parent, parentEntity);
	ASSERT_EQ(child2.parent, parentEntity);
	ASSERT_EQ(child3.parent, parentEntity);

	ASSERT_FALSE(registry.isValid(child1.previous));
	ASSERT_FALSE(registry.isValid(child3.next));
	ASSERT_EQ(child1.next, child2Entity);
	ASSERT_EQ(child2.previous, child1Entity);
	ASSERT_EQ(child2.next, child3Entity);
	ASSERT_EQ(child2.previous, child1Entity);
	ASSERT_EQ(child3.previous, child2Entity);
}

TEST(RegistryRelation, ChildrenLoop)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	for (size_t i = 0; i < 100; i++)
		ecs::Parent::createChild(registry, parentEntity);

	const auto& parent = registry.getComponent<const ecs::Parent>(parentEntity);
	ASSERT_EQ(parent.childrenCount, 100);

	auto n = 0;
	ecs::Parent::forEachChild(registry, parentEntity, [&](auto entity, const auto& child) {
		ASSERT_TRUE(registry.isValid(entity)) << "Child number " << static_cast<size_t>(n) << " is invalid";
		ASSERT_EQ(child.parent, parentEntity);
		n++;
	});

	ASSERT_EQ(n, 100);
}

TEST(RegistryRelation, RemoveOneChild)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	auto child1Entity = ecs::Parent::createChild(registry, parentEntity);
	auto child2Entity = ecs::Parent::createChild(registry, parentEntity);
	auto child3Entity = ecs::Parent::createChild(registry, parentEntity);

	ecs::Child::remove(registry, child1Entity);

	ASSERT_FALSE(registry.hasAnyComponent<ecs::Child>(child1Entity));

	const auto& parent = registry.getComponent<ecs::Parent>(parentEntity);
	ASSERT_EQ(parent.childrenCount, 2);

	const auto& child2 = registry.getComponent<ecs::Child>(child2Entity);
	const auto& child3 = registry.getComponent<ecs::Child>(child3Entity);

	ASSERT_FALSE(registry.isValid(child2.previous));
	ASSERT_EQ(child2.next, child3Entity);

	ASSERT_EQ(child3.previous, child2Entity);
	ASSERT_FALSE(registry.isValid(child3.next));
}

TEST(RegistryRelation, RemoveSecondChild)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	auto child1Entity = ecs::Parent::createChild(registry, parentEntity);
	auto child2Entity = ecs::Parent::createChild(registry, parentEntity);
	auto child3Entity = ecs::Parent::createChild(registry, parentEntity);

	ecs::Child::remove(registry, child2Entity);

	ASSERT_FALSE(registry.hasAnyComponent<ecs::Child>(child2Entity));

	const auto& parent = registry.getComponent<ecs::Parent>(parentEntity);
	ASSERT_EQ(parent.childrenCount, 2);

	const auto& child1 = registry.getComponent<ecs::Child>(child1Entity);
	const auto& child3 = registry.getComponent<ecs::Child>(child3Entity);

	ASSERT_FALSE(registry.isValid(child1.previous));
	ASSERT_EQ(child1.next, child3Entity);

	ASSERT_EQ(child3.previous, child1Entity);
	ASSERT_FALSE(registry.isValid(child3.next));
}

TEST(RegistryRelation, RemoveThirdChild)
{
	auto registry = ecs::Registry{};

	auto parentEntity = registry.createEntity();
	auto child1Entity = ecs::Parent::createChild(registry, parentEntity);
	auto child2Entity = ecs::Parent::createChild(registry, parentEntity);
	auto child3Entity = ecs::Parent::createChild(registry, parentEntity);

	ecs::Child::remove(registry, child3Entity);

	ASSERT_FALSE(registry.hasAnyComponent<ecs::Child>(child3Entity));

	const auto& parent = registry.getComponent<ecs::Parent>(parentEntity);
	ASSERT_EQ(parent.childrenCount, 2);

	const auto& child1 = registry.getComponent<ecs::Child>(child1Entity);
	const auto& child2 = registry.getComponent<ecs::Child>(child2Entity);

	ASSERT_FALSE(registry.isValid(child1.previous));
	ASSERT_EQ(child1.next, child2Entity);

	ASSERT_EQ(child2.previous, child1Entity);
	ASSERT_FALSE(registry.isValid(child2.next));
}

TEST(RegistryRelation, IsChild)
{
	auto registry = ecs::Registry{};

	auto e1 = registry.createEntity();
	auto e2 = ecs::Parent::createChild(registry, e1);
	auto e3 = ecs::Parent::createChild(registry, e2);
	auto e4 = ecs::Parent::createChild(registry, e3);
	auto e5 = ecs::Parent::createChild(registry, e1);
	auto e6 = registry.createEntity();

	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e1));

	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e2));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e2, e1));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e3));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e3, e1));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e2, e3));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e3, e2));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e3, e4));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e4, e3));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e4));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e4, e1));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e5));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e5, e1));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e2, e5));

	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e6));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e6, e1));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e6, e3));

	ecs::Child::remove(registry, e2);
	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e2));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e2, e3));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e3));
}

TEST(RegistryRelation, RemoveParent)
{
	auto registry = ecs::Registry{};

	auto e1 = registry.createEntity();
	auto e2 = ecs::Parent::createChild(registry, e1);
	auto e3 = ecs::Parent::createChild(registry, e1);

	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e2));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e3));

	ecs::Parent::remove(registry, e1);

	ASSERT_FALSE(registry.hasAnyComponent<ecs::Parent>(e1));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e2));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e3));
}

TEST(RegistryRelation, DestroyChildren)
{
	auto registry = ecs::Registry{};

	auto e1 = registry.createEntity();
	auto e2 = ecs::Parent::createChild(registry, e1);
	auto e3 = ecs::Parent::createChild(registry, e1);

	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e2));
	ASSERT_TRUE(ecs::Parent::isChild(registry, e1, e3));

	ecs::Parent::destroyChildren(registry, e1);

	ASSERT_FALSE(registry.isValid(e2));
	ASSERT_FALSE(registry.isValid(e3));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e2));
	ASSERT_FALSE(ecs::Parent::isChild(registry, e1, e3));
}