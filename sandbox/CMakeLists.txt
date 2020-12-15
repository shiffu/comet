
add_executable(sandbox
    sandboxApp.cpp
    sandboxMainScene.cpp
    main.cpp
)

target_link_libraries(sandbox PRIVATE comet)
