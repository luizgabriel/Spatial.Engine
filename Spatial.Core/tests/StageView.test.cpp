#include <fmt/format.h>
#include <gtest/gtest.h>
#include <spatial/render/InstanceBuilder.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/SceneNodeName.h>
#include <spatial/render/Stage.h>

using namespace spatial;

TEST(StageView, CountInstances)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	for (int i = 0; i < 10; i++)
		stage.createInstance(fmt::format("Test {}", i));

	for (int i = 0; i < 10; i++)
		stage.createInstance();

	ASSERT_EQ(20, stage.getInstancesCount());
	ASSERT_EQ(10, stage.getInstances<SceneNodeName>().size());
}

TEST(StageView, LoopForComponentForeach)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto stage = Stage{renderingSystem.getEngine()};

	for (int i = 0; i < 10; i++)
		createInstance(stage, fmt::format("Test {}", i));

	stage.getInstances<SceneNodeName>().each([](auto& name) { ASSERT_GT(name.value.size(), 0); });
}
