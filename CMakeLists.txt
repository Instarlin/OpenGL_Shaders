cmake_minimum_required(VERSION 3.30)

project(3DSceneGLFW VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(SOURCES
    main.cpp
    ./include/imgui/imgui.cpp
    ./include/imgui/imgui_draw.cpp
    ./include/imgui/imgui_tables.cpp
    ./include/imgui/imgui_widgets.cpp
    ./include/imgui/backend/imgui_impl_glfw.cpp
    ./include/imgui/backend/imgui_impl_opengl3.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ./include
    ./include/imgui
    ./include/imgui/backends
)

target_link_options(${PROJECT_NAME} PRIVATE -mwindows)

find_package(glfw3 REQUIRED)
find_library(GLEW_LIBRARY glew32 PATHS ${GLEW_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLEW_LIBRARY} opengl32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLEW_LIBRARY_DIR}/mingw64/bin/glew32.dll"
        "${GLFW_LIBRARY_DIR}/mingw64/bin/glfw3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
