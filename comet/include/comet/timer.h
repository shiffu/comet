#pragma once

#include <chrono>
#include <sstream>
#include <comet/log.h>

namespace comet
{
    using TimerClock = std::chrono::steady_clock;
    using TimerDuration = std::chrono::duration<long, std::nano>;
    using TimerTimePoint = std::chrono::time_point<TimerClock, TimerDuration>;

    class Timer
    {
    public:
        Timer(const char* name) : m_name(name)
        {
            start();
        }

        ~Timer()
        {
            stop();
        }

        void start()
        {
            m_startTime = TimerClock::now();
        }

        void pause()
        {
            if (m_paused == false)
            {
                m_endTime = TimerClock::now();
                m_accumulatedDuration +=  m_endTime - m_startTime;
                m_paused = true;
            }
        }
        
        void resume()
        {
            if (m_paused)
            {
                m_paused = false;
                m_startTime = TimerClock::now();
            }
        }
        
        void stop()
        {
            TimerDuration duration;
            if (m_paused == false)
            {
                m_endTime = TimerClock::now();
                duration =  m_accumulatedDuration + (m_endTime - m_startTime);
            }
            else
            {
                duration =  m_accumulatedDuration;
            }

            std::stringstream ss;
            ss << duration.count() * 0.000001f << "ms" << " - TIMER: " << m_name;
            CM_CORE_LOG_DEBUG(ss.str());
        }

    private:
        TimerTimePoint m_startTime;
        TimerTimePoint m_endTime;
        TimerDuration m_accumulatedDuration{0};
        const char* m_name;
        bool m_paused{false};
    };

#define COMET_PROFILE 1
#ifdef COMET_PROFILE
    #define CM_PROFILE_SCOPE(name) comet::Timer timer##__LINE__(name)
    #ifdef _WIN32
        #define CM_PROFILE_FUNC() comet::Timer timer##__LINE__(__funcsig__)
    #else
        #define CM_PROFILE_FUNC() comet::Timer timer##__LINE__(__PRETTY_FUNCTION__)
    #endif
#else
    #define CM_PROFILE_SCOPE(name)
#endif
    
} // namespace comet