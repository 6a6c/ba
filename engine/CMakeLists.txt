cmake_minimum_required(VERSION 3.5)
project(ba) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(VENDORED "Use vendored lib" ON)

if(VENDORED)
    # This assumes the SDL source is available in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    
    find_package(SDL3 REQUIRED)
endif()


add_library(matrix src/matrix.cpp)
target_include_directories(matrix PUBLIC include)

add_library(geometry src/geometry.cpp)
target_link_libraries(geometry PRIVATE matrix)
target_include_directories(geometry PUBLIC include)

add_library(engine src/engine.cpp)
target_link_libraries(engine PRIVATE matrix)
target_link_libraries(engine PRIVATE geometry)
target_link_libraries(engine PRIVATE SDL3::SDL3)
target_include_directories(engine PUBLIC include)
