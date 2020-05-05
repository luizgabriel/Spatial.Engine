#pragma once

#include <functional>

namespace spatial
{

int tryRun(const std::function<int(void)>& action);

} // namespace spatial
