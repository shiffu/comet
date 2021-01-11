cmake_minimum_required(VERSION 3.17)

add_executable(cometEditor
    panels/sceneHierarchyPanel.h
    panels/sceneHierarchyPanel.cpp
    panels/propertiesPanel.h
    panels/propertiesPanel.cpp
    cometEditorApp.h
    cometEditorApp.cpp
    cometEditorScene.h
    cometEditorScene.cpp
    main.cpp
)

target_precompile_headers(cometEditor REUSE_FROM comet)

target_include_directories(cometEditor
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_features(cometEditor PRIVATE cxx_std_17)

target_link_libraries(cometEditor
    PRIVATE
        comet
        ${CMAKE_DL_LIBS}
)
