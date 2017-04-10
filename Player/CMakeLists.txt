cmake_minimum_required (VERSION 3.4)

set(SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    GameTree.cpp
    GameTree.h
    Sequence.h
    Sequence.inl
    TranspositionTable.cpp
    TranspositionTable.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Misc/include
)

add_library(player ${SOURCES})
target_include_directories(player PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})
