#pragma once

#include <spdlog/spdlog.h>

namespace spatial
{

using Logger = spdlog::logger;

Logger createDefaultLogger() noexcept;

Logger createDefaultLogger(const std::string& output) noexcept;

} // namespace spatial