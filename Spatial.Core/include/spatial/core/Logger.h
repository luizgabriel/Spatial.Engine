#pragma once

#include <spdlog/spdlog.h>

namespace spatial
{

using Logger = spdlog::logger;

Logger createDefaultLogger(const std::string& output);

} // namespace spatial