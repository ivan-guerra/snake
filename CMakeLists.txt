cmake_minimum_required(VERSION 3.13...3.22)

project(snake
    DESCRIPTION "Snake for the Terminal"
)

set(SNAKE_BIN_DIR "${CMAKE_SOURCE_DIR}/bin"
    CACHE STRING  "${PROJECT_NAME} binary directory.")
set(SNAKE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include"
    CACHE STRING      "${PROJECT_NAME} include directory.")

add_subdirectory(src)
