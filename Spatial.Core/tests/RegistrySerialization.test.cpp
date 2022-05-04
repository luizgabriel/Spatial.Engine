#include <gtest/gtest.h>
#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/Registry.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>
#include <spatial/serialization/Snapshot.h>
#include <sstream>

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
	ecs::EntityBuilder::create(registry).withName("Entity 1").asTransform().withPosition(5.0f).asPointLight();

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
	ecs::EntityBuilder::create(registry)
				  .withName("Entity 1")
				  .asTransform()
				  .withPosition(5.0f)
				  .asPointLight()
				  .with<CustomComponent>();

	auto ss = std::stringstream{};

	{
		auto archive = XMLOutputArchive{ss};
		auto snapshot = ecs::Snapshot{registry};
		ASSERT_NO_THROW(snapshot.entities(archive));
		ASSERT_NO_THROW(ecs::serializeCoreComponents(archive, snapshot));
	}
}