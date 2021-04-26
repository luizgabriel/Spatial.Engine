#include <fstream>
#include <gtest/gtest.h>
#include <spatial/render/InstanceBuilder.h>
#include <spatial/render/Light.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Stage.h>
#include <spatial/serialization/Archives.h>

using namespace spatial;

TEST(Stage, EmptySerialization)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};
	auto ss = std::stringstream{};
	auto archive = JsonOuputArchive{ ss };
	archive(Archives{stage});

	ASSERT_EQ(ss.str(), "{}");
}
