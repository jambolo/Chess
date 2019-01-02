cmake_minimum_required (VERSION 3.4)

#option(FEATURE_PRIORITIZED_MOVE_ORDERING     "Prioritized move ordering is used if true"        FALSE)
#option(FEATURE_INCREMENTAL_STATIC_EVALUATION "Incremental static evaluation is enabled if true" FALSE)
#option(FEATURE_BITBOARD_THREAT_DETECTION     "Threat detection is done with bitboards if true"  FALSE)
option(ANALYSIS_GAME_STATE          "General GameState analysis is enabled if true"          FALSE)

message(STATUS "FEATURE_PRIORITIZED_MOVE_ORDERING                  : ${FEATURE_PRIORITIZED_MOVE_ORDERING}")
message(STATUS "FEATURE_INCREMENTAL_STATIC_EVALUATION              : ${FEATURE_INCREMENTAL_STATIC_EVALUATION}")
message(STATUS "FEATURE_BITBOARD_THREAT_DETECTION                  : ${FEATURE_BITBOARD_THREAT_DETECTION}")
message(STATUS "ANALYSIS_GAME_STATE                                : ${ANALYSIS_GAME_STATE}")

set(FEATURE_NOTATION "Standard" CACHE STRING "Which form of algrebric notation used?")
set_property(CACHE FEATURE_NOTATION PROPERTY STRINGS Standard Long Figurine UCI ICCF PGN)

message(STATUS "Notation is                                        : ${FEATURE_NOTATION}")

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
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(game_state ${SOURCES})
target_include_directories(game_state
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)

target_link_libraries(game_state
    zhash
    misc
    nlohmann_json::nlohmann_json
)

if(FEATURE_PRIORITIZED_MOVE_ORDERING)
    target_compile_definitions(game_state PUBLIC FEATURE_PRIORITIZED_MOVE_ORDERING=1)
endif()

if(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    target_compile_definitions(game_state PUBLIC FEATURE_INCREMENTAL_STATIC_EVALUATION=1)
endif()

if(FEATURE_NOTATION STREQUAL "Long")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_LONG=1)
elseif(FEATURE_NOTATION STREQUAL "Figurine")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_FIGURINE=1)
elseif(FEATURE_NOTATION STREQUAL "UCI")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_UCI=1)
elseif(FEATURE_NOTATION STREQUAL "ICCF")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_ICCF=1)
elseif(FEATURE_NOTATION STREQUAL "PGN")
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_PGN=1)
else()
    target_compile_definitions(game_state PRIVATE FEATURE_NOTATION_STANDARD=1)
endif()

if(ANALYSIS_GAME_STATE)
    target_compile_definitions(game_state PUBLIC ANALYSIS_GAME_STATE=1)
endif()
