
set(COMET_PUBLIC_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(COMET_SOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

add_library(comet STATIC
    # Headers
    ${COMET_SOURCES_DIR}/vendors/glad/include/glad/glad.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/log.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/application.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/window.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/comet.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/event.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/input.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertexBuffer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/indexBuffer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertexBufferLayout.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertexArray.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/shader.h
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLWindow.h

    # Sources
    ${COMET_SOURCES_DIR}/vendors/glad/src/glad.c
    ${COMET_SOURCES_DIR}/core/log.cpp
    ${COMET_SOURCES_DIR}/core/application.cpp
    ${COMET_SOURCES_DIR}/core/window.cpp
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLWindow.cpp
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLInput.h
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLInput.cpp
    ${COMET_SOURCES_DIR}/rendering/vertexBuffer.cpp
    ${COMET_SOURCES_DIR}/rendering/indexBuffer.cpp
    ${COMET_SOURCES_DIR}/rendering/vertexBufferLayout.cpp
    ${COMET_SOURCES_DIR}/rendering/vertexArray.cpp
    ${COMET_SOURCES_DIR}/rendering/shader.cpp
)

# For Debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

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

find_package(OpenGL REQUIRED)

# Link
target_include_directories(comet
    PUBLIC
        ${COMET_PUBLIC_HEADERS_DIR}
        ${COMET_SOURCES_DIR}/vendors/glad/include
        ${COMET_SOURCES_DIR}/vendors/glm
    PRIVATE
        ${COMET_SOURCES_DIR}
)

target_link_libraries(comet
    PUBLIC
        spdlog
    PRIVATE
        sfml-window
        OpenGL::GL
        ${CMAKE_DL_LIBS}
)

include(CMakePrintHelpers)
cmake_print_properties(
    TARGETS comet
    PROPERTIES
        INCLUDE_DIRECTORIES
        LINK_LIBRARIES
        SOURCE_DIR
)