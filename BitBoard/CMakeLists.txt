cmake_minimum_required (VERSION 3.4)

set(SOURCES
    BitBoard.cpp
    BitBoard.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(bitboard ${SOURCES})
target_include_directories(bitboard
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)
target_link_libraries(bitboard chess misc)

