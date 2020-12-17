#pragma once

#include <comet/platform.h>
#include <comet/log.h>

#define ASSERTION_ENABLED 1

#ifdef ASSERTION_ENABLED

    #ifdef _MSC_VER
        #include <intrin.h>
        #define debugBreak() __debugbreak()
    #elif __GNUC__
        #define debugBreak() __asm__("int $0x3")
    #elif __clang__
        #define debugBreak()
    #endif

    #define GET_3RD_ARG(arg1, arg2, arg3, ...) arg3

    #define ASSERT_CHOOSER(...) \
        GET_3RD_ARG(__VA_ARGS__, ASSERT_2_ARGS, ASSERT_1_ARGS, )

    #define ASSERT_1_ARGS(expr) \
        if (expr) {} \
        else \
        { \
            CM_CORE_LOG_FATAL("ASSERT {} failed in {}, line: {}", #expr, __FILE__, __LINE__); \
            debugBreak(); \
        }

    #define ASSERT_2_ARGS(expr, msg) \
        if (expr) {} \
        else \
        { \
            CM_CORE_LOG_FATAL("ASSERT: {} in {}, line: {}", msg, __FILE__, __LINE__); \
            debugBreak(); \
        }
    
    #define ASSERT(...) ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#else
    #define ASSERT(expr)
#endif