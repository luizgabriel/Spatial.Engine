#include <gtest/gtest.h>
#include <spatial/render/RenderingSystem.h>

using namespace spatial;

TEST(RenderingSystem, PushAndPopView)
{
	auto renderingSystem = RenderingSystem{RenderingSystem::Backend::NOOP, nullptr};
	auto view = renderingSystem.createView();

	ASSERT_EQ(0, renderingSystem.getViewsCount());

	renderingSystem.pushBackView(view);

	ASSERT_EQ(1, renderingSystem.getViewsCount());
}