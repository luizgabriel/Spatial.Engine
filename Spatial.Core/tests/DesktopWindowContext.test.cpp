#include <spatial/desktop/Window.h>
#include <gtest/gtest.h>

TEST(DesktopWindow, Create)
{
	auto context = spatial::DesktopPlatformContext{};
	auto window = context.createWindow(1280, 720, "Test");

	auto [w, h] = window.getWindowSize();
	ASSERT_EQ(w, 1280);
	ASSERT_EQ(h, 720);
	ASSERT_TRUE(window.getNativeHandle());
}
