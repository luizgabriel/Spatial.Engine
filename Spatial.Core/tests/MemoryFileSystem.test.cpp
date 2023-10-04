#include <spatial/resources/MemoryFileSystem.h>
#include <gtest/gtest.h>

using namespace spatial;


TEST(MemoryFileSystem, OpenFileForRead)
{
	auto f1 = MemoryFileSystem{};
	f1.define("test.txt", "Hello, World!");

	EXPECT_EQ(f1.readString("test.txt"), "Hello, World!");
}

TEST(MemoryFileSystem, ListPaths)
{
	auto f1 = MemoryFileSystem{};
	f1.define("a.txt", "A");
	f1.define("b.txt", "B");

	auto result = f1.list("/");
	EXPECT_EQ(result.size(), 2);
	EXPECT_TRUE(result.find({"a.txt", FileSystem::EntryType::File}) != result.end());
	EXPECT_TRUE(result.find({"b.txt", FileSystem::EntryType::File}) != result.end());
}