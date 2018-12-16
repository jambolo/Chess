cmake_minimum_required (VERSION 3.4)

set(SOURCES
    ZHash.cpp
    ZHash.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(zhash ${SOURCES})
target_include_directories(zhash
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)

target_link_libraries(zhash game_state misc)
