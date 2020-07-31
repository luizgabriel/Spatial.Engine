#include <chrono>
#include <spatial/core/Application.h>
#include <spatial/core/Logger.h>
#include <thread>

using namespace std::chrono_literals;

namespace spatial
{

Application::Application()
	: mRunning{false},
	  mClock{},
	  mDesiredDelta{1.0f / 60.0f},
	  mStartSignal{},
	  mStartFrameSignal{},
	  mUpdateFrameSignal{},
	  mEndFrameSignal{},
	  mFinishSignal{}
{
}

Application::~Application()
{
}

void Application::onEvent(const WindowClosedEvent& event)
{
	stop();
}

void Application::stop()
{
	mRunning = false;
}

int Application::run()
{
	mRunning = true;

	mStartSignal();

	while (mRunning)
	{
		const auto delta = mClock.getDeltaTime().count();

		mStartFrameSignal(delta);

		while (mClock.hasLag(mDesiredDelta))
		{
			mUpdateFrameSignal(mDesiredDelta);
			mClock.fixLag();
		}

		mEndFrameSignal();

		std::this_thread::sleep_until(mClock.getLastTime() + Clock<float>::delta_t{mDesiredDelta});
		mClock.tick();
	}

	mFinishSignal();

	return 0;
}

void Application::setMaxFps(float fps)
{
	mDesiredDelta = 1.0f / fps;
}

} // namespace spatial
