#pragma once

#include <spdlog/spdlog.h>

namespace spatial::common
{

class Logger
{
private:
    static spdlog::logger s_logger;

public:
    template <typename... Args>
    static void trace(Args... args)
    {
        s_logger.trace(std::template forward<Args>(args)...);
    }

    template <typename... Args>
    static void info(Args... args)
    {
        s_logger.info(std::template forward<Args>(args)...);
    }

    template <typename... Args>
    static void warn(Args... args)
    {
        s_logger.warn(std::template forward<Args>(args)...);
    }

    template <typename... Args>
    static void error(Args... args)
    {
        s_logger.error(std::template forward<Args>(args)...);
    }

    template <typename... Args>
    static void critical(Args... args)
    {
        s_logger.critical(std::template forward<Args>(args)...);
    }
};

} // namespace spatial::common