#pragma once

#include <chrono>

namespace spatial::core
{

using delta_t = std::chrono::duration<float>;


class Clock
{
public:
  Clock();

  void tick();
  delta_t getDeltaTime() const;

  std::chrono::steady_clock::time_point getLastTime() const
  {
    return m_LastTime;
  }

private:
  std::chrono::steady_clock::duration m_TimeStep;
  std::chrono::steady_clock::time_point m_LastTime;
};

} // namespace spatial::core
