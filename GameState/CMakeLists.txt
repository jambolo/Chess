cmake_minimum_required (VERSION 3.4)

option(FEATURE_PRIORITIZED_MOVE_ORDERING     "Prioritized move ordering is used if true"        FALSE)
option(FEATURE_INCREMENTAL_STATIC_EVALUATION "Incremental static evaluation is enabled if true" FALSE)

message(STATUS "FEATURE_PRIORITIZED_MOVE_ORDERING is ${FEATURE_PRIORITIZED_MOVE_ORDERING}")
message(STATUS "FEATURE_INCREMENTAL_STATIC_EVALUATION is ${FEATURE_INCREMENTAL_STATIC_EVALUATION}")

option(ANALYSIS_GAME_STATE "GameState analysis is enabled if true" FALSE)

message(STATUS "ANALYSIS_GAME_STATE is ${ANALYSIS_GAME_STATE}")

set(FEATURE_NOTATION "Standard" CACHE STRING "Which form of algrebric notation used?")
set_property(CACHE FEATURE_NOTATION PROPERTY STRINGS Standard Long Figurine UCI ICCF PGN)

message(STATUS "Notation is '${FEATURE_NOTATION}'")

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
)

add_library(game_state ${SOURCES})
target_include_directories(game_state PRIVATE ${PRIVATE_INCLUDE_PATHS})
target_link_libraries(game_state zhash misc nlohmann_json::nlohmann_json)

if(ANALYSIS_GAME_STATE)
    target_compile_definitions(game_state PUBLIC ANALYSIS_GAME_STATE=1)
endif()

if(FEATURE_PRIORITIZED_MOVE_ORDERING)
    target_compile_definitions(game_state PUBLIC FEATURE_PRIORITIZED_MOVE_ORDERING=1)
endif()

if(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    target_compile_definitions(game_state PUBLIC FEATURE_INCREMENTAL_STATIC_EVALUATION=1)
endif()

if(FEATURE_NOTATION EQUAL "Long")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_LONG=1)
elseif(FEATURE_NOTATION EQUAL "Figurine")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_FIGURINE=1)
elseif(FEATURE_NOTATION EQUAL "UCI")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_UCI=1)
elseif(FEATURE_NOTATION EQUAL "ICCF")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_ICCF=1)
elseif(FEATURE_NOTATION EQUAL "PGN")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_PGN=1)
else()
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_STANDARD=1)
endif()
