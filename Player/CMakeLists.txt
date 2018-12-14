cmake_minimum_required (VERSION 3.4)

set(SOURCES
    Player.cpp
    Player.h
)

set(PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(player ${SOURCES})
target_include_directories(player PRIVATE ${PRIVATE_INCLUDE_PATHS})
target_link_libraries(player game_state)
