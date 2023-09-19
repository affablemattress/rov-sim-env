cmake_minimum_required(VERSION 3.22)
project(cmake-boilerplate)

#util.a
add_library(util STATIC src/util.cpp)

target_include_directories(util 
    PUBLIC include/)

target_compile_definitions(util 
    PUBLIC "$<$<CONFIG:Debug>:NDEBUG>")

set_target_properties(util PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin/Debug/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin/Release/lib")

#main.out
add_executable(main src/main.cpp src/util.cpp)

target_include_directories(main 
    PUBLIC include/)
    
target_compile_definitions(main 
    PUBLIC "$<$<CONFIG:Debug>:NDEBUG>")

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin/Release"
    SUFFIX "${CMAKE_BUILD_TYPE}.out")

target_link_libraries(main util)

#misc
set_property(TARGET main util PROPERTY CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED 20)
add_compile_options(-Wall)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
