cmake_minimum_required(VERSION 3.17)

# Options
option(COMET_BUILD_WITH_GLFW "Build GLFW Library" ON)
option(COMET_BUILD_WITH_SFML "Build SFML Library" OFF)

# For Debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(comet SHARED)

if(COMET_BUILD_WITH_GLFW)
target_compile_definitions(comet PRIVATE COMET_WINDOW_IMPL=COMET_WINDOW_IMPL_GLFW)
message(STATUS "Option BUILD_GLFW was set to ON")
elseif(COMET_BUILD_WITH_SFML)
target_compile_definitions(comet PRIVATE COMET_WINDOW_IMPL=COMET_WINDOW_IMPL_SFML)
message(STATUS "Option BUILD_SFML was set to ON")
else()
message(FATAL_ERROR "Either COMET_BUILD_WITH_GLFW or COMET_BUILD_WITH_SFML option should be set to ON")
endif()


target_compile_features(comet PRIVATE cxx_std_17)
target_precompile_headers(comet PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/core/cometpch.h>")


target_sources(comet
    INTERFACE
        include/comet/comet.h

    PUBLIC
        include/comet/log.h
        include/comet/assert.h
        include/comet/utils.h
        include/comet/platform.h
        include/comet/graphicApiConfig.h
        include/comet/application.h
        include/comet/singleton.h
        include/comet/resourceManager.h
        include/comet/timer.h
        include/comet/window.h
        include/comet/event.h
        include/comet/input.h
        include/comet/framebuffer.h
        include/comet/shader.h
        include/comet/shaderRegistry.h
        include/comet/camera.h
        include/comet/cameraController.h
        include/comet/renderer.h
        include/comet/material.h
        include/comet/materialRegistry.h
        include/comet/staticMesh.h
        include/comet/vertex.h
        include/comet/light.h
        include/comet/directionalLight.h
        include/comet/pointLight.h
        include/comet/spotLight.h
        include/comet/texture.h
        include/comet/textureRegistry.h
        include/comet/entity.h
        include/comet/scene.h
        include/comet/components.h
        include/comet/nativeScript.h
        include/comet/propertiesExposer.h

    PRIVATE
        src/core/log.cpp
        src/core/application.cpp
        src/core/window.cpp
        src/core/resourceManager.cpp
        src/core/objLoader.h
        src/core/objLoader.cpp
        src/core/stringUtils.h
        src/core/stringUtils.cpp
        src/core/sceneSerializer.h
        src/core/sceneSerializer.cpp

        src/platforms/opengl/openglVertexBuffer.h
        src/platforms/opengl/openglVertexBuffer.cpp
        src/platforms/opengl/openglIndexBuffer.h
        src/platforms/opengl/openglIndexBuffer.cpp
        src/platforms/opengl/openglCommandBuffer.h
        src/platforms/opengl/openglCommandBuffer.cpp
        src/platforms/opengl/openglFramebuffer.h
        src/platforms/opengl/openglFramebuffer.cpp
        src/platforms/opengl/openglVertexArray.h
        src/platforms/opengl/openglVertexArray.cpp
        src/platforms/opengl/openglVertexBufferLayout.h
        src/platforms/opengl/openglVertexBufferLayout.cpp
        src/platforms/opengl/openglTexture.h
        src/platforms/opengl/openglTexture.cpp
        src/platforms/opengl/openglShader.h
        src/platforms/opengl/openglShader.cpp

        src/rendering/graphicApiConfig.cpp
        src/rendering/buffer.h
        src/rendering/vertexBuffer.h
        src/rendering/vertexBuffer.cpp
        src/rendering/indexBuffer.h
        src/rendering/indexBuffer.cpp
        src/rendering/commandBuffer.h
        src/rendering/commandBuffer.cpp
        src/rendering/framebuffer.cpp
        src/rendering/vertexBufferLayout.h
        src/rendering/vertexBufferLayout.cpp
        src/rendering/vertexArray.h
        src/rendering/vertexArray.cpp
        src/rendering/shader.cpp
        src/rendering/shaderRegistry.cpp
        src/rendering/camera.cpp
        src/rendering/cameraController.cpp
        src/rendering/renderer.cpp
        src/rendering/material.cpp
        src/rendering/directionalLight.cpp
        src/rendering/pointLight.cpp
        src/rendering/spotLight.cpp
        src/rendering/staticMesh.cpp
        src/rendering/imguiWrapper.h
        src/rendering/imguiWrapper.cpp
        src/rendering/texture.cpp
        src/rendering/textureRegistry.cpp

        src/ecs/scene.cpp

        src/scripting/nativeScript.cpp
        
        ## Vendors ##
        # glad
        src/vendors/glad/include/glad/glad.h
        src/vendors/glad/src/glad.c

        # ImGui
        src/vendors/imgui/imgui/imconfig.h
        src/vendors/imgui/imgui/imgui.cpp
        src/vendors/imgui/imgui/imgui.h
        src/vendors/imgui/imgui/imgui_draw.cpp
        src/vendors/imgui/imgui/imgui_widgets.cpp
        src/vendors/imgui/imgui/imgui_demo.cpp
        src/vendors/imgui/imgui/imgui_tables.cpp
        src/vendors/imgui/imgui/imgui_impl_opengl3.cpp
        src/vendors/imgui/imgui/imgui_impl_opengl3.h
        src/vendors/imgui/imgui/imgui_impl_glfw.cpp
        src/vendors/imgui/imgui/imgui_impl_glfw.h

        # stb
        src/vendors/stb/stb/stb_image.h
        src/vendors/stb/stb/stb_image.cpp

        # entt
        src/vendors/entt/entt/entt.hpp
 )

 # Only when BUILD_GLFW = ON
 if (COMET_BUILD_WITH_GLFW)
    target_sources(comet
        PRIVATE
            src/platforms/glfw/glfwWindowWrapper.cpp
            src/platforms/glfw/glfwInput.cpp
            src/platforms/glfw/glfwInput.h
            src/platforms/glfw/imguiGlfwWrapper.h
            src/platforms/glfw/imguiGlfwWrapper.cpp
    )
endif()

 # Only when BUILD_SFML = ON
 if (COMET_BUILD_WITH_SFML)
    target_sources(comet
        PRIVATE
            src/platforms/sfml/SFMLWindow.h
            src/platforms/sfml/SFMLWindow.cpp
            src/platforms/sfml/SFMLInput.h
            src/platforms/sfml/SFMLInput.cpp
    )
endif()

# Dependencies
#   spdlog
set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(src/vendors/spdlog)

#   Rapid Yaml (ryml)
set(RYML_BUILD_TESTS OFF)
set(RYML_BUILD_BENCHMARKS OFF)
add_subdirectory(src/vendors/ryml)
set_target_properties(ryml PROPERTIES POSITION_INDEPENDENT_CODE True)

#   GLFW
if (COMET_BUILD_WITH_GLFW)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    add_subdirectory(src/vendors/glfw)
endif()

#   SFML
if (COMET_BUILD_WITH_SFML)
    set(SFML_BUILD_EXAMPLES OFF)
    set(SFML_BUILD_DOC OFF)
    set(SFML_BUILD_AUDIO OFF)
    set(SFML_BUILD_NETWORK OFF)
    set(SFML_BUILD_GRAPHICS OFF)
    set(SFML_BUILD_WINDOW ON)
    add_subdirectory(src/vendors/sfml)
endif()

find_package(OpenGL REQUIRED)

# Link
target_include_directories(comet
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/glm
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/imgui
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/entt
        # TODO: Move this to private / build a more robust include structure
        ${CMAKE_CURRENT_LIST_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/glad/include
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/spdlog/include
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/stb
        ${CMAKE_CURRENT_LIST_DIR}/src/vendors/ryml/src
)

if (COMET_BUILD_WITH_GLFW)
    target_include_directories(comet
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src/vendors/glfw/include
    )
endif()

if (COMET_BUILD_WITH_SFML)
    target_include_directories(comet
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src/vendors/sfml/include
    )
endif()

target_link_libraries(comet
    PUBLIC
        spdlog
    PRIVATE
        stdc++fs
        OpenGL::GL
        ryml
        ${CMAKE_DL_LIBS}
)

if (COMET_BUILD_WITH_GLFW)
    target_link_libraries(comet
        PRIVATE
            glfw
    )
endif()

if (COMET_BUILD_WITH_SFML)
    target_link_libraries(comet
        PRIVATE
            sfml-window
    )
endif()


include(CMakePrintHelpers)
cmake_print_properties(
    TARGETS comet
    PROPERTIES
        INCLUDE_DIRECTORIES
        LINK_LIBRARIES
        SOURCE_DIR
        COMPILE_DEFINITIONS
)