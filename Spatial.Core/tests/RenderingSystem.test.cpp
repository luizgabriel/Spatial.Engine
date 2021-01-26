#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>

using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Invoke;
using ::testing::_;

using namespace spatial;

struct RenderingSystemMockListener {
	MOCK_METHOD1(onRender, void(filament::Renderer&));
};

TEST(RenderingSystem, TestOnRenderSignal)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto listener = RenderingSystemMockListener{};

	renderingSystem >> listener;
	renderingSystem.onEndFrame();

	EXPECT_CALL(listener, onRender(_)).Times(1);
}