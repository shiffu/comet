#include <comet/log.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace comet {

    std::shared_ptr<spdlog::logger> Log::s_cometLogger;
    std::shared_ptr<spdlog::logger> Log::s_applicationLogger;

    void Log::init()
    {
        spdlog::set_pattern("%^[%T] (%n:%l) %v%$");
        s_cometLogger = spdlog::stdout_color_mt("COMET");
        s_cometLogger->set_level(spdlog::level::trace);

        s_applicationLogger = spdlog::stdout_color_mt("APP");
        s_applicationLogger->set_level(spdlog::level::trace);
    }

} // namespace comet