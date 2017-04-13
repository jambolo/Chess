cmake_minimum_required (VERSION 3.4)

set(SOURCES
    Board.cpp
    Board.h
    Board.inl
    ChessTypes.h
    CMakeLists.txt
    GameState.cpp
    GameState.h
    GameState.inl
    Move.cpp
    Move.h
    Move.inl
    Piece.cpp
    Piece.h
    Position.cpp
    Position.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Misc/include
)

add_library(game_state ${SOURCES})
target_include_directories(game_state PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS} )