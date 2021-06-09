#include <gtest/gtest.h>
#include <entt/entity/snapshot.hpp>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>
#include <sstream>

using namespace spatial;

TEST(Registry, EmptySerialization)
{
	auto registry = ecs::Registry{};
	auto ss = std::stringstream {};

	{
		auto archive = XMLOutputArchive { ss };
		ecs::serialize(archive, registry);
	}

	auto output = ss.str();
	std::cout << output;

	ASSERT_NE(output.find("<cereal>"), std::string::npos);
	ASSERT_NE(output.find("</cereal>"), std::string::npos);
}

TEST(Registry, SimpleSerialization)
{
	auto registry = ecs::Registry{};
	auto e1 = ecs::createEntity(registry, "Entity 1");
	e1.add(ecs::Transform{math::float3{5.0f}});
	e1.add(ecs::PointLight{});

	auto ss = std::stringstream {};

	{
		auto archive = XMLOutputArchive { ss };
		ecs::serialize(archive, registry);
	}

	auto output = ss.str();
	std::cout << output;

	ASSERT_NE(output.find("<cereal>"), std::string::npos);
	ASSERT_NE(output.find("<position>"), std::string::npos);
	ASSERT_NE(output.find("<scale>"), std::string::npos);
	ASSERT_NE(output.find("<rotation>"), std::string::npos);
	ASSERT_NE(output.find("<intensity>"), std::string::npos);
}

struct CustomComponent {
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

	auto ss = std::stringstream {};

	{
		auto archive = XMLOutputArchive { ss };
		ecs::serialize<CustomComponent>(archive, registry);
	}

	auto output = ss.str();
	std::cout << output;

	ASSERT_NE(output.find("<cereal>"), std::string::npos);
	ASSERT_NE(output.find("<position>"), std::string::npos);
	ASSERT_NE(output.find("<scale>"), std::string::npos);
	ASSERT_NE(output.find("<rotation>"), std::string::npos);
	ASSERT_NE(output.find("<intensity>"), std::string::npos);
	ASSERT_NE(output.find("<answer>"), std::string::npos);
}