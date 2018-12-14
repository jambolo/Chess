cmake_minimum_required (VERSION 3.4)

option(FEATURE_TRANSPOSITION_TABLE                      "A transposition table is used if true"                          FALSE)
option(FEATURE_QUIESCENT_SEARCH                         "Quiescent search is enabled if true"                            FALSE)
option(FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION "Validation of incremental static evaluation is enabled if true" FALSE)

message(STATUS "FEATURE_TRANSPOSITION_TABLE is ${FEATURE_TRANSPOSITION_TABLE}")
message(STATUS "FEATURE_QUIESCENT_SEARCH is ${FEATURE_QUIESCENT_SEARCH}")
message(STATUS "FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION is ${FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION}")

option(ANALYSIS_PLAYER              "ComputerPlayer analysis is enabled if true"     FALSE)
option(ANALYSIS_TRANSPOSITION_TABLE "TranspositionTable analysis is enabled if true" FALSE)
option(ANALYSIS_GAME_TREE           "GameTree analysis is enabled if true"           FALSE)

message(STATUS "ANALYSIS_PLAYER is ${ANALYSIS_PLAYER}")
message(STATUS "ANALYSIS_TRANSPOSITION_TABLE is ${ANALYSIS_TRANSPOSITION_TABLE}")
message(STATUS "ANALYSIS_GAME_TREE is ${ANALYSIS_GAME_TREE}")

option(DEBUG_GAME_TREE_NODE_INFO "GameTree info is dumped if true" FALSE)

message(STATUS "DEBUG_GAME_TREE_NODE_INFO is ${DEBUG_GAME_TREE_NODE_INFO}")

set(SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    GameTree.cpp
    GameTree.h
    Sequence.cpp
    Sequence.h
    StaticEvaluator.cpp
    StaticEvaluator.h
    TranspositionTable.cpp
    TranspositionTable.h
)

set(PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(computer_player ${SOURCES})
target_include_directories(computer_player PRIVATE ${PRIVATE_INCLUDE_PATHS})
target_link_libraries(computer_player
    player
    game_state
    zhash
    nlohmann_json::nlohmann_json
)

if(FEATURE_TRANSPOSITION_TABLE)
    target_compile_definitions(computer_player PUBLIC FEATURE_TRANSPOSITION_TABLE=1)
endif()

if(FEATURE_QUIESCENT_SEARCH)
    target_compile_definitions(computer_player PUBLIC FEATURE_QUIESCENT_SEARCH=1)
endif()

if(FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION)
    target_compile_definitions(computer_player PUBLIC FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION=1)
endif()

if(ANALYSIS_PLAYER)
    target_compile_definitions(computer_player PUBLIC ANALYSIS_PLAYER=1)
endif()

if(ANALYSIS_TRANSPOSITION_TABLE)
    target_compile_definitions(computer_player PUBLIC ANALYSIS_TRANSPOSITION_TABLE=1)
endif()

if(ANALYSIS_GAME_TREE)
    target_compile_definitions(computer_player PUBLIC ANALYSIS_GAME_TREE=1)
endif()

if(DEBUG_GAME_TREE_NODE_INFO)
    target_compile_definitions(computer_player PUBLIC DEBUG_GAME_TREE_NODE_INFO=1)
endif()

