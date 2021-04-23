#include <spatial/desktop/Window.h>
#include <gtest/gtest.h>

using namespace spatial;

TEST(DesktopWindow, Create)
{
	auto context = desktop::PlatformContext{};
	auto window = context.createWindow(1280, 720, "Test");

	auto windowSize = window.getSize();
	ASSERT_EQ(windowSize.x, 1280);
	ASSERT_EQ(windowSize.y, 720);
	ASSERT_TRUE(window.getNativeHandle());
}
