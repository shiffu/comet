cmake_minimum_required(VERSION 3.17)

add_executable(cometEditor)

target_sources(cometEditor
    PRIVATE
        panels/sceneHierarchyPanel.h
        panels/sceneHierarchyPanel.cpp
        panels/propertiesPanel.h
        panels/propertiesPanel.cpp
        cometEditorApp.h
        cometEditorApp.cpp
        cometEditorScene.h
        cometEditorScene.cpp
        cometGameScene.h
        cometGameScene.cpp
        main.cpp

        # ImGui
        core/imguiUtils.h
        core/imguiWrapper.h
        core/imguiWrapper.cpp

        vendors/imgui/imgui/imconfig.h
        vendors/imgui/imgui/imgui.cpp
        vendors/imgui/imgui/imgui.h
        vendors/imgui/imgui/imgui_draw.cpp
        vendors/imgui/imgui/imgui_widgets.cpp
        vendors/imgui/imgui/imgui_demo.cpp
        vendors/imgui/imgui/imgui_tables.cpp
        vendors/imgui/imgui/imgui_impl_opengl3.cpp
        vendors/imgui/imgui/imgui_impl_opengl3.h
        vendors/imgui/imgui/imgui_impl_glfw.cpp
        vendors/imgui/imgui/imgui_impl_glfw.h        
)

 # Only when BUILD_GLFW = ON
 if (COMET_BUILD_WITH_GLFW)
    target_sources(cometEditor
        PRIVATE
            platforms/glfw/imguiGlfwWrapper.h
            platforms/glfw/imguiGlfwWrapper.cpp
    )
endif()

target_precompile_headers(cometEditor REUSE_FROM comet)

target_include_directories(cometEditor
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/vendors/imgui
        ${CMAKE_CURRENT_LIST_DIR}/../comet/src/vendors/glad/include
)

if (COMET_BUILD_WITH_GLFW)
    target_include_directories(cometEditor
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/../comet/src/vendors/glfw/include
    )
endif()

target_compile_features(cometEditor PRIVATE cxx_std_17)

target_link_libraries(cometEditor
    PRIVATE
        comet
        ${CMAKE_DL_LIBS}
)
