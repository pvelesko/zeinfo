cmake_minimum_required(VERSION 3.10)
project(zeinfo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_library(LevelZero_LIBRARY NAMES ze_loader PATHS ENV LD_LIBRARY_PATH NO_CACHE REQUIRED)
message(STATUS "LevelZero_LIBRARY: ${LevelZero_LIBRARY}")

add_executable(zeinfo zeinfo.cpp)
target_link_libraries(zeinfo ${LevelZero_LIBRARY})