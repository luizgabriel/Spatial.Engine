#include <spatial/common/Logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>

namespace spatial::common
{

spdlog::logger Logger::s_logger{[]{

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("[SPATIAL] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("spatial.log", true);
    file_sink->set_level(spdlog::level::warn);

    auto logger = spdlog::logger("spatial", { console_sink, file_sink });
    s_logger.set_level(spdlog::level::debug);

    return logger;

}()};

}