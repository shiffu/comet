
set(COMET_PUBLIC_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(COMET_SOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

add_library(comet STATIC
    # Public Headers
    ${COMET_SOURCES_DIR}/vendors/glad/include/glad/glad.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/log.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/assert.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/utils.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/platform.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/application.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/singleton.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/resourceManager.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/timer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/window.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/comet.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/event.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/input.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/buffer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertexBuffer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/indexBuffer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/commandBuffer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertexBufferLayout.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertexArray.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/shader.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/shaderRegistry.h
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLWindow.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/camera.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/cameraController.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/renderer.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/material.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/materialRegistry.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/flatColorMaterial.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/phongMaterial.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/staticMesh.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/vertex.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/drawingContext.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/light.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/directionalLight.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/pointLight.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/spotLight.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/texture.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/textureRegistry.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/entity.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/scene.h
    ${COMET_PUBLIC_HEADERS_DIR}/comet/components.h

    # Sources and private Headers
    ${COMET_SOURCES_DIR}/vendors/glad/src/glad.c
    ${COMET_SOURCES_DIR}/core/log.cpp
    ${COMET_SOURCES_DIR}/core/application.cpp
    ${COMET_SOURCES_DIR}/core/window.cpp
    ${COMET_SOURCES_DIR}/core/resourceManager.cpp
    ${COMET_SOURCES_DIR}/core/objLoader.h
    ${COMET_SOURCES_DIR}/core/objLoader.cpp
    ${COMET_SOURCES_DIR}/core/stringUtils.h
    ${COMET_SOURCES_DIR}/core/stringUtils.cpp
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLWindow.cpp
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLInput.h
    ${COMET_SOURCES_DIR}/platforms/sfml/SFMLInput.cpp
    ${COMET_SOURCES_DIR}/platforms/glfw/glfwWindowWrapper.cpp
    ${COMET_SOURCES_DIR}/platforms/glfw/glfwInput.cpp
    ${COMET_SOURCES_DIR}/platforms/glfw/glfwInput.h
    ${COMET_SOURCES_DIR}/rendering/buffer.cpp
    ${COMET_SOURCES_DIR}/rendering/vertexBuffer.cpp
    ${COMET_SOURCES_DIR}/rendering/indexBuffer.cpp
    ${COMET_SOURCES_DIR}/rendering/commandBuffer.cpp
    ${COMET_SOURCES_DIR}/rendering/vertexBufferLayout.cpp
    ${COMET_SOURCES_DIR}/rendering/vertexArray.cpp
    ${COMET_SOURCES_DIR}/rendering/shader.cpp
    ${COMET_SOURCES_DIR}/rendering/shaderRegistry.cpp
    ${COMET_SOURCES_DIR}/rendering/camera.cpp
    ${COMET_SOURCES_DIR}/rendering/cameraController.cpp
    ${COMET_SOURCES_DIR}/rendering/renderer.cpp
    ${COMET_SOURCES_DIR}/rendering/material.cpp
    ${COMET_SOURCES_DIR}/rendering/directionalLight.cpp
    ${COMET_SOURCES_DIR}/rendering/pointLight.cpp
    ${COMET_SOURCES_DIR}/rendering/spotLight.cpp
    ${COMET_SOURCES_DIR}/rendering/flatColorMaterial.cpp
    ${COMET_SOURCES_DIR}/rendering/phongMaterial.cpp
    ${COMET_SOURCES_DIR}/rendering/staticMesh.cpp
    ${COMET_SOURCES_DIR}/rendering/imguiWrapper.h
    ${COMET_SOURCES_DIR}/rendering/imguiWrapper.cpp
    ${COMET_SOURCES_DIR}/rendering/texture.cpp
    ${COMET_SOURCES_DIR}/rendering/textureRegistry.cpp
    ${COMET_SOURCES_DIR}/ecs/scene.cpp

    # ImGui
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imconfig.h
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui.cpp
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui.h
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_draw.cpp
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_widgets.cpp
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_demo.cpp
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_impl_opengl3.cpp
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_impl_opengl3.h
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_impl_glfw.cpp
    ${COMET_SOURCES_DIR}/vendors/imgui/imgui/imgui_impl_glfw.h
    ${COMET_SOURCES_DIR}/platforms/glfw/imguiGlfwWrapper.h
    ${COMET_SOURCES_DIR}/platforms/glfw/imguiGlfwWrapper.cpp

    # stb
    ${COMET_SOURCES_DIR}/vendors/stb/stb_image.h
    ${COMET_SOURCES_DIR}/vendors/stb/stb_image.cpp

    # entt
    ${COMET_SOURCES_DIR}/vendors/entt/entt/entt.hpp
 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Debugging
set(CMAKE_VERBOSE_MAKEFILE OFF)

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

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(src/vendors/glfw)

find_package(OpenGL REQUIRED)

# Link
target_include_directories(comet
    PUBLIC
        ${COMET_PUBLIC_HEADERS_DIR}
        ${COMET_SOURCES_DIR}/vendors/glad/include
        ${COMET_SOURCES_DIR}/vendors/glm
        ${COMET_SOURCES_DIR}/vendors/imgui
        ${COMET_SOURCES_DIR}/vendors/entt
    PRIVATE
        ${COMET_SOURCES_DIR}
        ${COMET_SOURCES_DIR}/vendors
)

target_link_libraries(comet
    PUBLIC
        spdlog
    PRIVATE
        stdc++fs
        glfw
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