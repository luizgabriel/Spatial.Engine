#pragma once

#include <spdlog/spdlog.h>

namespace spatial::core
{

using Logger = spdlog::logger;

Logger createDefaultLogger(const std::string& output);

} // namespace spatial::common