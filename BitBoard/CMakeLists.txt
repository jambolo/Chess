cmake_minimum_required (VERSION 3.4)

set(SOURCES
    BitBoard.cpp
    BitBoard.h
)

set(PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Misc/include
)

set(PUBLIC_INCLUDE_PATHS
    .
)

add_library(bitboard ${SOURCES})
target_include_directories(bitboard PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})
