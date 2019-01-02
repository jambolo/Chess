cmake_minimum_required (VERSION 3.4)

option(FEATURE_TRANSPOSITION_TABLE                        "A transposition table is used if true"                          FALSE)
#option(FEATURE_QUIESCENT_SEARCH                           "Quiescent search is enabled if true"                            FALSE)
#option(FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION   "Validation of incremental static evaluation is enabled if true" FALSE)
#option(FEATURE_TRANSPOSITION_TABLE_INCLUDES_PRUNED_VALUES "Transposition table includes results of pruned searches"        FALSE)
#option(FEATURE_ITERATIVE_DEEPENING                        "Iterative deepening is enabled if true"                         FALSE)
#option(FEATURE_INTERNAL_ITERATIVE_DEEPENING               "Internal iterative deepening is enabled if true"                FALSE)
#option(FEATURE_NULL_MOVE                                  "Null move search is enabled if true"                            FALSE)
#option(FEATURE_BITBOARD_MOVE_GENERATION                   "Move generation is done with bitboards if true"                 FALSE)

message(STATUS "FEATURE_TRANSPOSITION_TABLE                        : ${FEATURE_TRANSPOSITION_TABLE}")
message(STATUS "FEATURE_QUIESCENT_SEARCH                           : ${FEATURE_QUIESCENT_SEARCH}")
message(STATUS "FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION   : ${FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION}")
message(STATUS "FEATURE_TRANSPOSITION_TABLE_INCLUDES_PRUNED_VALUES : ${FEATURE_TRANSPOSITION_TABLE_INCLUDES_PRUNED_VALUES}")
message(STATUS "FEATURE_ITERATIVE_DEEPENING                        : ${FEATURE_ITERATIVE_DEEPENING}")
message(STATUS "FEATURE_INTERNAL_ITERATIVE_DEEPENING               : ${FEATURE_INTERNAL_ITERATIVE_DEEPENING}")
message(STATUS "FEATURE_NULL_MOVE                                  : ${FEATURE_NULL_MOVE}")
message(STATUS "FEATURE_BITBOARD_MOVE_GENERATION                   : ${FEATURE_BITBOARD_MOVE_GENERATION}")

option(ANALYSIS_PLAYER              "General ComputerPlayer analysis is enabled if true"     FALSE)
option(ANALYSIS_TRANSPOSITION_TABLE "General TranspositionTable analysis is enabled if true" FALSE)
option(ANALYSIS_GAME_TREE           "General GameTree analysis is enabled if true"           FALSE)

message(STATUS "ANALYSIS_PLAYER                                    : ${ANALYSIS_PLAYER}")
message(STATUS "ANALYSIS_TRANSPOSITION_TABLE                       : ${ANALYSIS_TRANSPOSITION_TABLE}")
message(STATUS "ANALYSIS_GAME_TREE                                 : ${ANALYSIS_GAME_TREE}")

#option(DEBUG_GAME_TREE_NODE_INFO "GameTree info is dumped if true" FALSE)

message(STATUS "DEBUG_GAME_TREE_NODE_INFO                          : ${DEBUG_GAME_TREE_NODE_INFO}")

set(SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    GameTree.cpp
    GameTree.h
    StaticEvaluator.cpp
    StaticEvaluator.h
    TranspositionTable.cpp
    TranspositionTable.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(computer_player ${SOURCES})
target_include_directories(computer_player
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)

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

