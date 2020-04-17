#include <spatial/core/Clock.h>

using namespace std::chrono;


namespace spatial
{

Clock::Clock() 
	: m_timeStep(steady_clock::duration::zero()),
	  m_lastTime(steady_clock::now())
{
}

void Clock::tick()
{
	m_timeStep = steady_clock::now() - m_lastTime;
	m_lastTime = steady_clock::now();
}

} // namespace spatial
