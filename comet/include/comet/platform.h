#pragma once


#if _WIN32 || _WIN64
    #define PLATFORM_WINDOWS
#elif __linux__
    #define PLATFORM_LINUX
#elif __APPLE__
    #define PLATFORM_MAC
#else
    #error "Unmanaged platform!"
#endif


#ifdef PLATFORM_WINDOWS
    #ifdef BUILD_DLL
        #define COMET_API __declspec(dllexport)
    #else
        #define COMET_API __declspec(dllimport)
    #endif
#else
    #define COMET_API
#endif


// Define the managed Window/Input platform specific implementations
#define COMET_WINDOW_IMPL_SFML  0
#define COMET_WINDOW_IMPL_GLFW  1

// Define the Platform Window Implementation to be used
#ifndef COMET_WINDOW_IMPL
    #define COMET_WINDOW_IMPL COMET_WINDOW_IMPL_GLFW
#endif
