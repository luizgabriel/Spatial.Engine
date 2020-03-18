#include <spatial/core/Clock.h>

namespace spatial::core
{

using namespace std::chrono;

Clock::Clock() : m_TimeStep(steady_clock::duration::zero()), m_LastTime(steady_clock::now())
{
}

void Clock::tick()
{
	m_TimeStep = steady_clock::now() - m_LastTime;
	m_LastTime = steady_clock::now();
}

delta_t Clock::getDeltaTime() const
{
	return duration_cast<duration<float>>(m_TimeStep);
}

} // namespace spatial::core
