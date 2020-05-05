#pragma once

#include <chrono>

namespace spatial
{

using delta_t = std::chrono::duration<float>;

class Clock
{
public:
	Clock();

	void tick();

	auto getLastTime() const { return m_lastTime; }

	auto getDeltaTime() const { return std::chrono::duration_cast<delta_t>(m_timeStep); }

private:
	std::chrono::steady_clock::duration m_timeStep;
	std::chrono::steady_clock::time_point m_lastTime;
};

} // namespace spatial
