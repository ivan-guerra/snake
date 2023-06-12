cmake_minimum_required(VERSION 3.13...3.22)

find_package(Curses REQUIRED)

project(screen
    DESCRIPTION "Implementation of on Screen Graphics Using ncurses"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE screen.cc
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CURSES_INCLUDE_DIR}
    PUBLIC ${SNAKE_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${CURSES_LIBRARIES}
    PRIVATE game
)
