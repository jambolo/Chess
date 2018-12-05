cmake_minimum_required (VERSION 3.4)

option(GAME_STATE_ANALYSIS_ENABLED           "GameState analysis is enabled if true"            FALSE)
option(USING_PRIORITIZED_MOVE_ORDERING       "Prioritized move ordering is used if true"        FALSE)
option(INCREMENTAL_STATIC_EVALUATION_ENABLED "Incremental static evaluation is enabled if true" FALSE)

set(NOTATION "Standard" CACHE STRING "Which form of algrebric notation used?")
set_property(CACHE NOTATION PROPERTY STRINGS Standard Long Figurine UCI ICCF PGN)
 
message(STATUS "Algebraic Notation = '${NOTATION}'")
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

if(GAME_STATE_ANALYSIS_ENABLED)
    target_compile_definitions(game_state PUBLIC GAME_STATE_ANALYSIS_ENABLED=1)
endif()

if(USING_PRIORITIZED_MOVE_ORDERING)
    target_compile_definitions(game_state PUBLIC USING_PRIORITIZED_MOVE_ORDERING=1)
endif()

if(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    target_compile_definitions(game_state PUBLIC INCREMENTAL_STATIC_EVALUATION_ENABLED=1)
endif()

if(NOTATION EQUAL "Long")
    target_compile_definitions(game_state PRIVATE NOTATION_LONG=1)
elseif(NOTATION EQUAL "Figurine")
    target_compile_definitions(game_state PRIVATE NOTATION_FIGURINE=1)
elseif(NOTATION EQUAL "UCI")
    target_compile_definitions(game_state PRIVATE NOTATION_UCI=1)
elseif(NOTATION EQUAL "ICCF")
    target_compile_definitions(game_state PRIVATE NOTATION_ICCF=1)
elseif(NOTATION EQUAL "PGN")
    target_compile_definitions(game_state PRIVATE NOTATION_PGN=1)
else()
    target_compile_definitions(game_state PRIVATE NOTATION_STANDARD=1)
endif()