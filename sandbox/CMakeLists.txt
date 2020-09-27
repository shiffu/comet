
add_executable(sandbox
    sandboxApp.cpp
    main.cpp
)

target_link_libraries(sandbox PRIVATE comet)
