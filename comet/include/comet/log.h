#pragma once

#include <memory>
#include <spdlog/logger.h>

namespace comet {

    class Log
    {
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger>& getCometLogger() { return s_cometLogger; }
        inline static std::shared_ptr<spdlog::logger>& getApplicationLogger() { return s_applicationLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_cometLogger;
        static std::shared_ptr<spdlog::logger> s_applicationLogger;
    };
} // namespace comet

#define CM_CORE_LOG_FATAL(...) comet::Log::getCometLogger()->critical(__VA_ARGS__)
#define CM_CORE_LOG_ERROR(...) comet::Log::getCometLogger()->error(__VA_ARGS__)
#define CM_CORE_LOG_WARN(...) comet::Log::getCometLogger()->warn(__VA_ARGS__)
#define CM_CORE_LOG_INFO(...) comet::Log::getCometLogger()->info(__VA_ARGS__)
#define CM_CORE_LOG_DEBUG(...) comet::Log::getCometLogger()->debug(__VA_ARGS__)
#define CM_CORE_LOG_TRACE(...) comet::Log::getCometLogger()->trace(__VA_ARGS__)

#define CM_LOG_FATAL(...) comet::Log::getApplicationLogger()->critical(__VA_ARGS__)
#define CM_LOG_ERROR(...) comet::Log::getApplicationLogger()->error(__VA_ARGS__)
#define CM_LOG_WARN(...) comet::Log::getApplicationLogger()->warn(__VA_ARGS__)
#define CM_LOG_INFO(...) comet::Log::getApplicationLogger()->info(__VA_ARGS__)
#define CM_LOG_DEBUG(...) comet::Log::getApplicationLogger()->debug(__VA_ARGS__)
#define CM_LOG_TRACE(...) comet::Log::getApplicationLogger()->trace(__VA_ARGS__)