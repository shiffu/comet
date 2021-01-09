cmake_minimum_required(VERSION 3.17)

add_library(testScriptsLib SHARED)

target_compile_features(testScriptsLib PRIVATE cxx_std_17)

# Sources
target_sources(testScriptsLib
    INTERFACE
        scripts.h

    PUBLIC
    PRIVATE
        scripts.cpp
 )

# Link
target_include_directories(testScriptsLib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
)

target_link_libraries(testScriptsLib
    PUBLIC
    PRIVATE
        comet
        stdc++fs
)
