#include <spatial/resources/PhysicalFileSystem.h>
#include <gtest/gtest.h>

using namespace spatial;


TEST(PhysicalFileSystem, OpenFileForRead)
{
	auto f1 = PhysicalFileSystem{std::filesystem::current_path() / "fs" / "a1"};
	EXPECT_EQ(f1.readString("b1/test.txt"), "Hello, World! Inside A1/B1");

	auto f2 = PhysicalFileSystem{std::filesystem::current_path() / "fs" / "a1" / "b1"};
	EXPECT_EQ(f2.readString("test.txt"), "Hello, World! Inside A1/B1");
}

TEST(PhysicalFileSystem, OpenFileForWrite)
{
	auto f1 = PhysicalFileSystem{std::filesystem::current_path() / "fs" / "a1"};
	EXPECT_TRUE(f1.write("b1/test_write.txt", "Test Write"));
	EXPECT_EQ(f1.readString("b1/test_write.txt"), "Test Write");
}

TEST(PhysicalFileSystem, ListPaths)
{
	auto root = PhysicalFileSystem{std::filesystem::current_path() / "fs"};

	{
		auto result = root.list("a1/");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"b1", FileSystem::EntryType::Directory}) != result.end());
	}

	{
		auto result = root.list("a2");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"test.txt", FileSystem::EntryType::File}) != result.end());
	}
}