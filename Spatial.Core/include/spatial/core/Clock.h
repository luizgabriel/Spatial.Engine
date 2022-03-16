#pragma once

#include <chrono>

namespace spatial
{

template <typename Precision = float>
class Clock
{
  public:
	using delta_t = std::chrono::duration<Precision>;

	Clock()
		: mTimeStep(std::chrono::steady_clock::duration::zero()), mLastTime(std::chrono::steady_clock::now()), mLag()
	{
	}

	void tick()
	{
		mTimeStep = std::chrono::steady_clock::now() - mLastTime;
		mLastTime = std::chrono::steady_clock::now();

		mLag += getDeltaTime();
	}

	bool hasLag(Precision desiredDeltaTime)
	{
		return mLag >= desiredDeltaTime;
	}

	void fixLag()
	{
		mLag -= getDeltaTime();
	}

	auto getLastTime() const
	{
		return mLastTime;
	}

	auto getDeltaTime() const
	{
		return std::chrono::duration_cast<delta_t>(mTimeStep);
	}

	template <typename Func>
	static delta_t measure(Func func)
	{
		auto clock = Clock{};
		func();
		clock.tick();
		return clock.getDeltaTime();
	}

  private:
	std::chrono::steady_clock::duration mTimeStep;
	std::chrono::steady_clock::time_point mLastTime;
	delta_t mLag;
};

} // namespace spatial
