#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>

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
	auto renderingSystem = render::RenderingSystem{nullptr};
	auto listener = RenderingSystemMockListener{};

	EXPECT_CALL(listener, onRender(_)).Times(1);

	renderingSystem >> listener;
	renderingSystem.onEndFrame();
}