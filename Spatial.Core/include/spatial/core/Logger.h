#pragma once

#include <spdlog/spdlog.h>

namespace spatial::core
{

spdlog::logger createDefaultLogger(const std::string& output);

} // namespace spatial::common