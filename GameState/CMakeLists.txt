cmake_minimum_required (VERSION 3.4)

set(SOURCES
    Bishop.cpp
    Bishop.h
    Board.cpp
    Board.h
    ChessTypes.h
    CMakeLists.txt
    GameState.cpp
    GameState.h
    King.cpp
    King.h
    Knight.cpp
    Knight.h
    Move.cpp
    Move.h
    Pawn.cpp
    Pawn.h
    Piece.cpp
    Piece.h
    Position.cpp
    Position.h
    Queen.cpp
    Queen.h
    Rook.cpp
    Rook.h
)

set(PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Misc/include
)

set(PUBLIC_INCLUDE_PATHS
    .
)

add_library(game_state ${SOURCES})
target_include_directories(game_state PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS} )