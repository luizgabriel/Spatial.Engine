#pragma once

#include <chrono>

namespace spatial
{

using dmilliseconds = std::chrono::duration<double, std::milli>;
using dseconds = std::chrono::duration<double>;
using dminutes = std::chrono::duration<double, std::ratio<60>>;
using dhours = std::chrono::duration<double, std::ratio<3600>>;
using ddays = std::chrono::duration<double, std::ratio<86400>>;
using dweeks = std::chrono::duration<double, std::ratio<604800>>;
using dmonths = std::chrono::duration<double, std::ratio<2629746>>;
using dyears = std::chrono::duration<double, std::ratio<31556952>>;

// or

using fmilliseconds = std::chrono::duration<float, std::milli>;
using fseconds = std::chrono::duration<float>;
using fminutes = std::chrono::duration<float, std::ratio<60>>;
using fhours = std::chrono::duration<float, std::ratio<3600>>;
using fdays = std::chrono::duration<float, std::ratio<86400>>;
using fweeks = std::chrono::duration<float, std::ratio<604800>>;
using fmonths = std::chrono::duration<float, std::ratio<2629746>>;
using fyears = std::chrono::duration<float, std::ratio<31556952>>;

class Clock
{
  public:
	Clock()
		: mTimeStep{std::chrono::steady_clock::duration::zero()},
		  mLastTime{std::chrono::steady_clock::now()},
		  mLag{std::chrono::steady_clock::duration::zero()}
	{
	}

	void reset()
	{
		mTimeStep = std::chrono::steady_clock::duration::zero();
		mLag = std::chrono::steady_clock::duration::zero();
	}

	template <typename DurationT = fseconds>
	[[nodiscard]] DurationT getElapsedTime() const
	{
		return std::chrono::duration_cast<DurationT>(mTimeStep);
	}

	void tick()
	{
		mTimeStep = std::chrono::steady_clock::now() - mLastTime;
		mLastTime = std::chrono::steady_clock::now();
		mLag += mTimeStep;
	}

	bool hasLag(std::chrono::steady_clock::duration desiredDeltaTime)
	{
		return mLag >= desiredDeltaTime;
	}

	void fixLag()
	{
		mLag -= mTimeStep;
	}

	template <typename DurationT = fseconds, typename Func>
	static auto measure(Func func)
	{
		auto clock = Clock{};
		func();
		clock.tick();
		return clock.template getElapsedTime<DurationT>();
	}

  private:
	std::chrono::steady_clock::duration mTimeStep;
	std::chrono::steady_clock::time_point mLastTime;
	std::chrono::steady_clock::duration mLag;
};

} // namespace spatial
