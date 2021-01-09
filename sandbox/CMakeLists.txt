cmake_minimum_required(VERSION 3.17)

add_executable(sandbox
    sandboxApp.cpp
    sandboxMainScene.cpp
    main.cpp
)

target_compile_features(sandbox PRIVATE cxx_std_17)

target_link_libraries(sandbox
    PRIVATE
        comet
        ${CMAKE_DL_LIBS}
)
