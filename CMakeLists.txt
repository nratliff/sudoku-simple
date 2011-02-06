cmake_minimum_required(VERSION 2.6)
project(SUDOKU)

set(PROJECT_DIR ${SUDOKU_SOURCE_DIR})
set(PROJECT_BINARY_DIR ${PROJECT_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release)
#add_definitions(-O3)


# Include the base source directory so we can reference headers 
# from the source root.
include_directories(
    ${PROJECT_DIR}/src
)

add_subdirectory(src)


