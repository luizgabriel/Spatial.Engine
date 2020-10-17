#include <spatial/assets/PhysicalDirLoader.h>
#include <gtest/gtest.h>

TEST(PhysicalDirLoader, Load)
{
	auto loader = spatial::assets::PhysicalDirLoader{"assets"};
	auto resource = loader.load("test.txt");
	ASSERT_TRUE(resource.has_value());

	auto value = std::string{resource->begin(), resource->end()};
	ASSERT_EQ(value, "Hello, world!");
}
