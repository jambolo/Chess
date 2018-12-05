cmake_minimum_required (VERSION 3.4)

set(SOURCES
    BitBoard.cpp
    BitBoard.h
)

set(PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(bitboard ${SOURCES})
target_include_directories(bitboard PRIVATE ${PRIVATE_INCLUDE_PATHS})
target_link_libraries(bitboard game_state misc)

