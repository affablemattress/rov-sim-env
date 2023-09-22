cmake_minimum_required(VERSION 3.22)
project(cmake-boilerplate)

#spdlog.a
    add_subdirectory("vendor/spdlog")

    set_target_properties(spdlog PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/lib")

#imgui.a
    add_library(imgui STATIC 
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_tables.cpp)

    target_include_directories(imgui PUBLIC 
        vendor/imgui
        vendor/imgui/backends)

    set_target_properties(imgui PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/lib")

#glfw.a
    add_subdirectory("vendor/glfw")

    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    set_target_properties(glfw PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/lib")

#glad.a
    add_library(glad STATIC 
        vendor/glad/src/glad.c)

    target_include_directories(glad PUBLIC 
        vendor/glad/include)

    set_target_properties(glad PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/lib")

#main.out
add_executable(main src/main.cpp)

target_include_directories(main PUBLIC
    include/)

target_include_directories(main PUBLIC 
    vendor/glad/include 
    vendor/glfw/include 
    vendor/imgui 
    vendor/imgui/backends 
    vendor/spdlog/include)

target_compile_definitions(main 
    PUBLIC "$<$<CONFIG:Debug>:NDEBUG>")

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release"
    SUFFIX ".out")

target_link_libraries(main glad glfw spdlog)

#misc
set_property(TARGET main glad glfw PROPERTY CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED 20)
add_compile_options(-Wall)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
