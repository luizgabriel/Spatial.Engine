#include <gtest/gtest.h>
#include <spatial/filesystem/MemoryFileSystem.h>
#include <spatial/filesystem/PhysicalFileSystem.h>

using namespace spatial;
using namespace std::string_literals;
using namespace std::string_view_literals;
namespace fs = spatial::filesystem;

TEST(PhysicalFileSystem, ReadAndWrite)
{
	auto fileSystem = fs::PhysicalFileSystem{"./"};
	const auto data = "Hello, world!\n"s;

	EXPECT_NO_THROW(fileSystem.write("test.txt", data.begin(), data.end()));

	EXPECT_NO_THROW({
		auto result = std::string{};
		fileSystem.read("test.txt", std::back_inserter(result));
		ASSERT_EQ(result, "Hello, world!\n");
		fileSystem.read("test.txt", std::back_inserter(result));
		ASSERT_EQ(result, "Hello, world!\nHello, world!\n");
	});
}

TEST(MemoryFileSystem, ReadAndWrite)
{
	auto fileSystem = fs::MemoryFileSystem{};
	const auto data = "Hello, world!\n"s;

	EXPECT_NO_THROW(fileSystem.write("test.txt", data.begin(), data.end()));

	EXPECT_NO_THROW({
		auto result = std::string{};
		fileSystem.read("test.txt", std::back_inserter(result));
		ASSERT_EQ(result, "Hello, world!\n");
		fileSystem.read("test.txt", std::back_inserter(result));
		ASSERT_EQ(result, "Hello, world!\nHello, world!\n");
	});
}