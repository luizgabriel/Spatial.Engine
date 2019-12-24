#include <spatial/physics/SimulationSubsystem.h>

namespace spatial::physics
{

using namespace std::chrono;

SimulationSubsystem::SimulationSubsystem()
    : m_TimeStep(steady_clock::duration::zero()),
      m_LastTime(steady_clock::now())
{
}

void SimulationSubsystem::process()
{
    m_TimeStep = steady_clock::now() - m_LastTime;
    m_LastTime = steady_clock::now();
}

delta_t SimulationSubsystem::getDeltaTime() const
{
    return duration_cast<duration<float>>(m_TimeStep);
}

} // namespace spatial::physics
