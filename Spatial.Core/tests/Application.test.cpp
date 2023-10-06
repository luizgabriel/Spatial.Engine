#include <chrono>
#include <spatial/core/Application.h>
#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std::chrono_literals;
using ::testing::_;
using ::testing::AtLeast;

std::thread startApplicationRuntime(spatial::Application& app)
{
	auto runtime = std::thread([&]() { app.run(); });
	while (!app.isRunning())
		;
	return runtime;
}

void simulateRuntime(spatial::Application& app, const std::chrono::nanoseconds& duration)
{
	auto runtime = startApplicationRuntime(app);
	std::this_thread::sleep_for(duration);
	app.onEvent(spatial::desktop::WindowClosedEvent{nullptr});
	runtime.join();
}

struct ApplicationMockListener
{
	MOCK_METHOD0(onStart, void());
	MOCK_METHOD1(onStartFrame, void(float));
	MOCK_METHOD1(onUpdateFrame, void(float));
};

TEST(Application, OnStartListener)
{
	auto app = spatial::Application{};
	auto listener = ApplicationMockListener{};

	EXPECT_CALL(listener, onStart()).Times(1);

	app.getStartSignal().connect<&ApplicationMockListener::onStart>(listener);
	simulateRuntime(app, 1ms);
}

TEST(Application, OnStartFrameListener)
{
	auto app = spatial::Application{};
	auto listener = ApplicationMockListener{};

	EXPECT_CALL(listener, onStartFrame(_)).Times(AtLeast(1));

	app.getStartFrameSignal().connect<&ApplicationMockListener::onStartFrame>(listener);
	simulateRuntime(app, 1ms);
}

TEST(Application, OnUpdateFrameListener)
{
	auto app = spatial::Application{};
	auto listener = ApplicationMockListener{};

	EXPECT_CALL(listener, onUpdateFrame(_)).Times(AtLeast(2));

	app.getUpdateFrameSignal().connect<&ApplicationMockListener::onUpdateFrame>(listener);
	simulateRuntime(app, 500ms);
}