#include <spatial/resources/AggregateFileSystem.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <spatial/resources/PhysicalFileSystem.h>
#include <gtest/gtest.h>

using namespace spatial;


TEST(AggregateFileSystem, OpenFileForRead)
{
	auto f1 = AggregateFileSystem{};
	auto a = std::make_shared<MemoryFileSystem>();
	a->define("a.txt", "Hello, World!");

	auto b = std::make_shared<PhysicalFileSystem>(std::filesystem::current_path() / "fs" / "a1");

	f1.mount("a", a);
	f1.mount("b", b);

	EXPECT_EQ(f1.readString("a/a.txt"), "Hello, World!");
	EXPECT_EQ(f1.readString("b/b1/test.txt"), "Hello, World! Inside A1/B1");
}

TEST(AggregateFileSystem, ListPaths)
{
	auto f1 = AggregateFileSystem{};
	auto a = std::make_shared<MemoryFileSystem>();
	a->define("a.txt", "Hello, World!");

	auto b = std::make_shared<PhysicalFileSystem>(std::filesystem::current_path() / "fs" / "a2");

	f1.mount("a", a);
	f1.mount("b", b);

	{
		auto result = f1.list("/");
		EXPECT_EQ(result.size(), 2);
		EXPECT_TRUE(result.find({"a", FileSystem::EntryType::Directory}) != result.end());
		EXPECT_TRUE(result.find({"b", FileSystem::EntryType::Directory}) != result.end());
	}

	{
		auto result = f1.list("a");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"a.txt", FileSystem::EntryType::File}) != result.end());
	}

	{
		auto result = f1.list("b");
		EXPECT_EQ(result.size(), 1);
		EXPECT_TRUE(result.find({"test.txt", FileSystem::EntryType::File}) != result.end());
	}
}