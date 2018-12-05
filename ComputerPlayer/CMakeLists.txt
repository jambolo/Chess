cmake_minimum_required (VERSION 3.4)

option(FEATURE_GAME_TREE_ANALYSIS                       "GameTree analysis is enabled if true"                           FALSE)
option(FEATURE_TRANSPOSITION_TABLE                      "A transposition table is used if true"                          FALSE)
option(FEATURE_DEBUG_GAME_TREE_NODE_INFO                "GameTree info is dumped if true"                                FALSE)
option(FEATURE_TRANSPOSITION_TABLE_ANALYSIS             "TranspositionTable analysis is enabled if true"                 FALSE)
option(FEATURE_QUIESCENT_SEARCH                         "Quiescent search is enabled if true"                            FALSE)
option(FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION "Validation of incremental static evaluation is enabled if true" FALSE)

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
    ${CMAKE_SOURCE_DIR}/Misc/include
)

set(PUBLIC_INCLUDE_PATHS
    .
)

add_library(computer_player ${SOURCES})
target_include_directories(computer_player PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})

if(FEATURE_GAME_TREE_ANALYSIS)
    target_compile_definitions(computer_player PUBLIC FEATURE_GAME_TREE_ANALYSIS=1)
endif()

if(FEATURE_TRANSPOSITION_TABLE)
    target_compile_definitions(computer_player PUBLIC FEATURE_TRANSPOSITION_TABLE=1)
endif()

if(FEATURE_DEBUG_GAME_TREE_NODE_INFO)
    target_compile_definitions(computer_player PUBLIC FEATURE_DEBUG_GAME_TREE_NODE_INFO=1)
endif()

if(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
    target_compile_definitions(computer_player PUBLIC FEATURE_TRANSPOSITION_TABLE_ANALYSIS=1)
endif()

if(FEATURE_QUIESCENT_SEARCH)
    target_compile_definitions(computer_player PUBLIC FEATURE_QUIESCENT_SEARCH=1)
endif()

if(FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION)
    target_compile_definitions(computer_player PUBLIC FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION=1)
endif()
