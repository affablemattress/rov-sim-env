cmake_minimum_required(VERSION 3.22)
project(cmake-boilerplate)

#spdlog.a
    add_subdirectory("vendor/spdlog")

    set_target_properties(spdlog PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/lib")

#imgui.a
    add_library(imgui STATIC 
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp)

    target_include_directories(imgui PUBLIC 
        vendor/imgui
        vendor/imgui/backends
        vendor/glfw/include)

    set_target_properties(imgui PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/lib")

#glfw.a
    add_subdirectory("vendor/glfw")

    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    set_target_properties(glfw PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/lib")

#glad.a
    add_library(glad STATIC 
        vendor/glad/src/glad.c)

    target_include_directories(glad PUBLIC 
        vendor/glad/include)

    set_target_properties(glad PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release/lib")

#main.out
    add_executable(main 
        src/main.cpp 
        src/callbackGLFW.cpp 
        src/lifetime.cpp 
        src/shader.cpp 
        src/camera.cpp 
        src/window.cpp 
        src/gui.cpp 
        src/math.cpp
        src/object.cpp)

    target_link_libraries(main glad glfw spdlog imgui)

    target_include_directories(main PUBLIC
        "include/"
        vendor/glad/include 
        vendor/glfw/include 
        vendor/imgui 
        vendor/spdlog/include 
        vendor/stb 
        vendor/glm)

    set_target_properties(main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release"
        SUFFIX ".out")

    target_compile_definitions(main 
        PUBLIC "$<$<CONFIG:Debug>:NDEBUG>")

#misc
    set_property(GLOBAL PROPERTY CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED 20)
    add_compile_options(-Wall)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
