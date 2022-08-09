#include <gtest/gtest.h>
#include <spatial/ecs/Resource.h>

using namespace spatial;

TEST(RegistryResource, OnlyFile)
{
	auto resource = ecs::Resource{"file.txt"};
	ASSERT_EQ(resource.filename(), "file.txt");
	ASSERT_EQ(resource.extension(), ".txt");
	ASSERT_EQ(resource.stem(), "file");
}

TEST(RegistryResource, Empty)
{
	auto resource = ecs::Resource{""};
	ASSERT_EQ(resource.filename(), "");
	ASSERT_EQ(resource.extension(), "");
	ASSERT_EQ(resource.stem(), "");
}

TEST(RegistryResource, BasicPath)
{
	auto resource = ecs::Resource{"some/test/folder/file.txt"};
	ASSERT_EQ(resource.filename(), "file.txt");
	ASSERT_EQ(resource.extension(), ".txt");
	ASSERT_EQ(resource.stem(), "file");
}

TEST(RegistryResource, StrangeFileName)
{
	auto resource = ecs::Resource{"some/test/folder/image..txt..png"};
	ASSERT_EQ(resource.filename(), "image..txt..png");
	ASSERT_EQ(resource.extension(), "..txt..png");
	ASSERT_EQ(resource.stem(), "image");
}

TEST(RegistryResource, NoExtension)
{
	auto resource = ecs::Resource{"some/test/folder/noext"};
	ASSERT_EQ(resource.filename(), "noext");
	ASSERT_EQ(resource.extension(), "");
	ASSERT_EQ(resource.stem(), "noext");
}

TEST(RegistryResource, HiddenFile)
{
	auto resource = ecs::Resource{"some/test/folder/.hidden"};
	ASSERT_EQ(resource.filename(), ".hidden");
	ASSERT_EQ(resource.extension(), ".hidden");
	ASSERT_EQ(resource.stem(), "");
}

TEST(RegistryResource, StrangePath)
{
	auto resource = ecs::Resource{"C:\\some//a/image..txt..jpg"};
	ASSERT_EQ(resource.filename(), "image..txt..jpg");
	ASSERT_EQ(resource.extension(), "..txt..jpg");
	ASSERT_EQ(resource.stem(), "image");
}