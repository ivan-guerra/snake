cmake_minimum_required(VERSION 3.13...3.22)

find_package(Curses REQUIRED)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE snake.cc
)

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE ${CURSES_INCLUDE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE ${CURSES_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${SNAKE_BIN_DIR}"
)
