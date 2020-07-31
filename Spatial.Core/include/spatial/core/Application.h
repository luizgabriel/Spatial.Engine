#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/common/Signal.h>
#include <spatial/core/Clock.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

class Application final
{
  private:
	bool mRunning;

	Clock<float> mClock;
	float mDesiredDelta;

	// region Signals
	Signal<> mStartSignal;

	Signal<float> mStartFrameSignal;
	Signal<float> mUpdateFrameSignal;
	Signal<> mEndFrameSignal;

	Signal<> mFinishSignal;
	// endregion

  public:
	Application();
	~Application();

	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;

	int run();

	void stop();

	bool isRunning() const
	{
		return mRunning;
	}

	void onEvent(const WindowClosedEvent& event);

	void setMaxFps(float fps);

	auto& getStartSignal()
	{
		return mStartSignal;
	}

	auto& getFinishSignal()
	{
		return mFinishSignal;
	}

	auto& getStartFrameSignal()
	{
		return mStartFrameSignal;
	}

	auto& getUpdateFrameSignal()
	{
		return mUpdateFrameSignal;
	}

	auto& getEndFrameSignal()
	{
		return mEndFrameSignal;
	}
};

} // namespace spatial
