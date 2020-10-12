#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

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

TEST(Application, CreateEmpty)
{
	auto app = spatial::Application{};
	ASSERT_EQ(app.isRunning(), false);
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

//TODO: Use GMock instead
struct MockListener {
	int onStartCalledTimes = 0;
	int onStartFrameCalledTimes = 0;

	void onStart() {
		onStartCalledTimes++;
	};

	void onStartFrame(float) {
		onStartFrameCalledTimes++;
	}
};

TEST(Application, OnStartListener)
{
	auto app = spatial::Application{};
	auto listener = MockListener{};

	ASSERT_EQ(listener.onStartCalledTimes, 0);

	app >> listener;
	simulateRuntime(app, 1ms);

	ASSERT_EQ(listener.onStartCalledTimes, 1);
}

TEST(Application, OnStartFrameListener)
{
	auto app = spatial::Application{};
	auto listener = MockListener{};

	ASSERT_EQ(listener.onStartFrameCalledTimes, 0);

	app >> listener;
	simulateRuntime(app, 1ms);

	ASSERT_GT(listener.onStartFrameCalledTimes, 0);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
