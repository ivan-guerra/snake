cmake_minimum_required(VERSION 3.13...3.22)

project(game
    DESCRIPTION "Implementation of Snake"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${SNAKE_INCLUDE_DIR}
)

target_sources(${PROJECT_NAME}
    PRIVATE game.cc
)
