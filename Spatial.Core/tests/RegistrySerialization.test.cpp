#include <gtest/gtest.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>
#include <sstream>
#include "spatial/serialization/Snapshot.h"

using namespace spatial;

TEST(Registry, EmptySerialization)
{
	auto registry = ecs::Registry{};
	auto ss = std::stringstream{};

	{
		auto archive = XMLOutputArchive{ss};
		auto snapshot = ecs::Snapshot{registry};
		ASSERT_NO_THROW(snapshot.entities(archive));
		ASSERT_NO_THROW(ecs::serializeCoreComponents(archive, snapshot));
	}
}

TEST(Registry, SimpleSerialization)
{
	auto registry = ecs::Registry{};
	auto e1 = ecs::createEntity(registry, "Entity 1");
	e1.add(ecs::Transform{math::float3{5.0f}});
	e1.add(ecs::PointLight{});

	auto ss = std::stringstream{};

	{
		auto archive = XMLOutputArchive{ss};
		auto snapshot = ecs::Snapshot{registry};
		ASSERT_NO_THROW(snapshot.entities(archive));
		ASSERT_NO_THROW(ecs::serializeCoreComponents(archive, snapshot));
	}
}

struct CustomComponent
{
	int answer{42};

	template <typename Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(answer));
	}
};

TEST(Registry, CustomComponentSerialization)
{
	auto registry = ecs::Registry{};
	auto e1 = ecs::createEntity(registry, "Entity 1");
	e1.add(ecs::Transform{math::float3{5.0f}});
	e1.add(ecs::PointLight{});
	e1.add(CustomComponent{});

	auto ss = std::stringstream{};

	{
		auto archive = XMLOutputArchive{ss};
		auto snapshot = ecs::Snapshot{registry};
		ASSERT_NO_THROW(snapshot.entities(archive));
		ASSERT_NO_THROW(ecs::serializeCoreComponents(archive, snapshot));
	}
}