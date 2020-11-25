#pragma once

#include <platforms/platform.h>


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

    #define ASSERT(expr) \
        if (expr) {} \
        else \
        { \
            CM_CORE_LOG_FATAL("assert: {} failed in {}, line: {}", #expr, __FILE__, __LINE__); \
            debugBreak(); \
        }
#else
    #define ASSERT(expr)
#endif