#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spatial/graphics/RenderingSystem.h>
#include <spatial/graphics/RenderingSystemUtils.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Invoke;

using namespace spatial;

struct RenderingSystemMockListener
{
	MOCK_METHOD1(onRender, void(filament::Renderer&));
};

TEST(RenderingSystem, TestOnRenderSignal)
{
	auto renderingSystem = graphics::RenderingSystem{nullptr};
	auto listener = RenderingSystemMockListener{};

	EXPECT_CALL(listener, onRender(_)).Times(1);

	renderingSystem >> listener;
	renderingSystem.onEndFrame();
}