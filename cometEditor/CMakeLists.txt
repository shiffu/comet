cmake_minimum_required(VERSION 3.17)

add_executable(cometEditor
    panels/sceneHierarchyPanel.h
    panels/sceneHierarchyPanel.cpp
    panels/propertiesPanel.h
    panels/propertiesPanel.cpp
    cometEditorApp.h
    cometEditorApp.cpp
    main.cpp
)

target_include_directories(cometEditor
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_features(cometEditor PRIVATE cxx_std_17)

target_link_libraries(cometEditor PRIVATE comet)
