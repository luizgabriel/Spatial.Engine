#include <spatial/core/Application.h>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std::chrono_literals;
using ::testing::AtLeast;
using ::testing::_;

std::thread startApplicationRuntime(spatial::Application& app)
{
	auto runtime = std::thread([&]() { app.run(); });
	while (!app.isRunning());
	return runtime;
}

void simulateRuntime(spatial::Application& app, const std::chrono::nanoseconds& duration) {
	auto runtime = startApplicationRuntime(app);
	std::this_thread::sleep_for(duration);
	app.onEvent(spatial::WindowClosedEvent{});
	runtime.join();
}

TEST(Application, CreateAndStop)
{
	auto app = spatial::Application{};
	ASSERT_EQ(app.isRunning(), false);

	auto runtime = startApplicationRuntime(app);
	ASSERT_EQ(app.isRunning(), true);

	app.onEvent(spatial::WindowClosedEvent{});
	runtime.join();

	ASSERT_EQ(app.isRunning(), false);
}

struct MockListener {
	MOCK_METHOD0(onStart, void());
	MOCK_METHOD1(onStartFrame, void(float));
	MOCK_METHOD1(onUpdateFrame, void(float));
};

TEST(Application, OnStartListener)
{
	auto app = spatial::Application{};
	auto listener = MockListener{};

	EXPECT_CALL(listener, onStart())
		.Times(1);

	app.getStartSignal().connect<&MockListener::onStart>(listener);
	simulateRuntime(app, 1ms);
}

TEST(Application, OnStartFrameListener)
{
	auto app = spatial::Application{};
	auto listener = MockListener{};

	EXPECT_CALL(listener, onStartFrame(_))
		.Times(AtLeast(1));

	app.getStartFrameSignal().connect<&MockListener::onStartFrame>(listener);
	simulateRuntime(app, 1ms);
}

TEST(Application, OnUpdateFrameListener)
{
	auto app = spatial::Application{};
	auto listener = MockListener{};

	EXPECT_CALL(listener, onUpdateFrame(_))
		.Times(AtLeast(2));

	app.getUpdateFrameSignal().connect<&MockListener::onUpdateFrame>(listener);
	simulateRuntime(app, 500ms);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
