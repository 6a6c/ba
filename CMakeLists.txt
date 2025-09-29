cmake_minimum_required(VERSION 3.10)
project(ba) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(engine)

# executable output
add_executable(ba main.cpp)

# Link to the actual SDL3 library.
target_link_libraries(ba PRIVATE engine)

include_guard(GLOBAL)
