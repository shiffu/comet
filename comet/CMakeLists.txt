
set(COMET_PUBLIC_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(COMET_SOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

add_library(comet STATIC
    # Headers
    ${COMET_PUBLIC_HEADERS_DIR}/comet/log.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/window.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/application.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/comet.h
    ${COMET_SOURCES_DIR}/platforms/window/SFMLWindow.h

    # Sources
    ${COMET_SOURCES_DIR}/core/application.cpp
    ${COMET_SOURCES_DIR}/core/log.cpp
    ${COMET_SOURCES_DIR}/core/window.cpp
    ${COMET_SOURCES_DIR}/core/windowImpl.h
    ${COMET_SOURCES_DIR}/events/event.h
    ${COMET_SOURCES_DIR}/platforms/window/SFMLWindow.cpp
)

# For Debugging
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Dependencies
#   spdlog
add_subdirectory(src/vendors/spdlog)

#   SFML
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOC OFF)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_WINDOW ON)

add_subdirectory(src/vendors/sfml)

# Link
target_include_directories(comet
    PUBLIC
        ${COMET_PUBLIC_HEADERS_DIR}
    PRIVATE
        ${COMET_SOURCES_DIR}
)

target_link_libraries(comet
    PUBLIC
        spdlog
    PRIVATE
        sfml-window
)

include(CMakePrintHelpers)
cmake_print_properties(
    TARGETS comet
    PROPERTIES
        INCLUDE_DIRECTORIES
        LINK_LIBRARIES
        SOURCE_DIR
)