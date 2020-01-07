#pragma once

#include <chrono>

namespace spatial::physics
{

using delta_t = std::chrono::duration<float>;

class SimulationSubsystem
{
public:
  SimulationSubsystem();

  void process();
  delta_t getDeltaTime() const;

  std::chrono::steady_clock::time_point getLastTime() const
  {
    return m_LastTime;
  }

private:
  std::chrono::steady_clock::duration m_TimeStep;
  std::chrono::steady_clock::time_point m_LastTime;
};

} // namespace spatial::physics
