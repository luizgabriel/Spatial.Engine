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
		: m_timeStep(std::chrono::steady_clock::duration::zero()), m_lastTime(std::chrono::steady_clock::now()), m_lag()
	{
	}

	void tick()
	{
		m_timeStep = std::chrono::steady_clock::now() - m_lastTime;
		m_lastTime = std::chrono::steady_clock::now();

		m_lag += getDeltaTime().count();
	}

	bool hasLag(Precision desiredDeltaTime)
	{
		return m_lag >= desiredDeltaTime;
	}

	void fixLag()
	{
		m_lag -= getDeltaTime().count();
	}

	auto getLastTime() const
	{
		return m_lastTime;
	}

	auto getDeltaTime() const
	{
		return std::chrono::duration_cast<delta_t>(m_timeStep);
	}

  private:
	std::chrono::steady_clock::duration m_timeStep;
	std::chrono::steady_clock::time_point m_lastTime;
	Precision m_lag;
};

} // namespace spatial
