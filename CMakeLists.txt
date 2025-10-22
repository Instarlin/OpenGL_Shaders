cmake_minimum_required(VERSION 3.28)
project(3DShader VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(imgui REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json REQUIRED)

set(CORE_SOURCES
    src/core/main.cpp
    src/core/application.cpp
)

set(CONFIG_SOURCES
    src/config/config_manager.cpp
)

set(UTILS_SOURCES
    src/utils/file_utils.cpp
    src/utils/error_utils.cpp
)

set(STATE_SOURCES
    src/state/app_state.cpp
)

set(SETTINGS_SOURCES
    src/settings/settings_state.cpp
    src/settings/settings_manager.cpp
)

set(RENDERING_SOURCES
    src/rendering/shader_manager.cpp
    src/rendering/renderer.cpp
)

set(WINDOW_SOURCES
    src/window/window_manager.cpp
    src/window/input_manager.cpp
)

set(UI_SOURCES
    src/ui/ui_manager.cpp
    src/backends/imgui_impl_glfw.cpp
    src/backends/imgui_impl_opengl3.cpp
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${CONFIG_SOURCES}
    ${UTILS_SOURCES}
    ${STATE_SOURCES}
    ${SETTINGS_SOURCES}
    ${RENDERING_SOURCES}
    ${WINDOW_SOURCES}
    ${UI_SOURCES}
)

file(GLOB_RECURSE SHADER_FILES "src/glsl/*.glsl" "src/glsl/*.vert" "src/glsl/*.frag")
file(GLOB_RECURSE ICON_FILES "ico/*.ico" "ico/*.png")

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    GLEW::GLEW
    imgui::imgui
    OpenGL::GL
    nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glsl
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/glsl
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/ico
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
)
