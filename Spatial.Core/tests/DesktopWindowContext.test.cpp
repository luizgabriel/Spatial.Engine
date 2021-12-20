#include <gtest/gtest.h>
#include <spatial/desktop/Window.h>

using namespace spatial;

TEST(DesktopWindow, Create)
{
	auto context = desktop::PlatformContext{};
	auto window = context.createWindow(230, 110, "Test");

	auto windowSize = window.getSize();
	ASSERT_EQ(windowSize.x, 230);
	ASSERT_EQ(windowSize.y, 110);
	ASSERT_TRUE(window.getNativeHandle());
}
