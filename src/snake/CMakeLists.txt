cmake_minimum_required(VERSION 3.13...3.22)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE snake.cc
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE game
    PRIVATE screen
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${SNAKE_BIN_DIR}"
)
