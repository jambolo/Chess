cmake_minimum_required (VERSION 3.4)

#option(FEATURE_PRIORITIZED_MOVE_ORDERING     "Prioritized move ordering is used if true"        FALSE)
#option(FEATURE_INCREMENTAL_STATIC_EVALUATION "Incremental static evaluation is enabled if true" FALSE)
#option(FEATURE_BITBOARD_THREAT_DETECTION     "Threat detection is done with bitboards if true"  FALSE)
#option(ANALYSIS_GAME_STATE                    "General GameState analysis is enabled if true"    FALSE)

message(STATUS "FEATURE_PRIORITIZED_MOVE_ORDERING                  : ${FEATURE_PRIORITIZED_MOVE_ORDERING}")
message(STATUS "FEATURE_INCREMENTAL_STATIC_EVALUATION              : ${FEATURE_INCREMENTAL_STATIC_EVALUATION}")
message(STATUS "FEATURE_BITBOARD_THREAT_DETECTION                  : ${FEATURE_BITBOARD_THREAT_DETECTION}")
message(STATUS "ANALYSIS_GAME_STATE                                : ${ANALYSIS_GAME_STATE}")

set(SOURCES
    Bishop.cpp
    Bishop.h
    Board.cpp
    Board.h
    GameState.cpp
    GameState.h
    King.cpp
    King.h
    Knight.cpp
    Knight.h
    Move.cpp
    Move.h
    ResponseGenerator.cpp
    ResponseGenerator.h
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
    StaticEvaluator.cpp
    StaticEvaluator.h
    Types.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(Chess ${SOURCES})
target_include_directories(Chess
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)

target_link_libraries(Chess PUBLIC
    GamePlayer
    Misc::Misc
    nlohmann_json::nlohmann_json
    ZHash
)

if(FEATURE_PRIORITIZED_MOVE_ORDERING)
    target_compile_definitions(Chess PUBLIC FEATURE_PRIORITIZED_MOVE_ORDERING=1)
endif()

if(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    target_compile_definitions(Chess PUBLIC FEATURE_INCREMENTAL_STATIC_EVALUATION=1)
endif()

if(ANALYSIS_GAME_STATE)
    target_compile_definitions(Chess PUBLIC ANALYSIS_GAME_STATE=1)
endif()

source_group(Sources FILES ${SOURCES})

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif()

