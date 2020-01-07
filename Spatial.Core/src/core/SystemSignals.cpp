#include <spatial/core/SystemSignals.h>

namespace spatial::core
{

common::Signal<float> SystemSignals::s_updateSignal{};
common::Signal<> SystemSignals::s_startSignal{};
common::Signal<> SystemSignals::s_finishSignal{};

}