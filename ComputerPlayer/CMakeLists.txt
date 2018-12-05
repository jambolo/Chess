cmake_minimum_required (VERSION 3.4)

option(GAME_TREE_ANALYSIS_ENABLED                       "GameTree analysis is enabled if true"                           FALSE)
option(USING_TRANSPOSITION_TABLE                        "A transposition table is used if true"                          FALSE)
option(PRINTING_GAME_TREE_NODE_INFO                     "GameTree info is dumped if true"                                FALSE)
option(TRANSPOSITION_TABLE_ANALYSIS_ENABLED             "TranspositionTable analysis is enabled if true"                 FALSE)
option(QUIESCENT_SEARCH_ENABLED                         "Quiescent search is enabled if true"                            FALSE)
option(INCREMENTAL_STATIC_EVALUATION_VALIDATION_ENABLED "Validation of incremental static evaluation is enabled if true" FALSE)

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

if(GAME_TREE_ANALYSIS_ENABLED)
    target_compile_definitions(computer_player PUBLIC GAME_TREE_ANALYSIS_ENABLED=1)
endif()

if(USING_TRANSPOSITION_TABLE)
    target_compile_definitions(computer_player PUBLIC USING_TRANSPOSITION_TABLE=1)
endif()

if(PRINTING_GAME_TREE_NODE_INFO)
    target_compile_definitions(computer_player PUBLIC PRINTING_GAME_TREE_NODE_INFO=1)
endif()

if(TRANSPOSITION_TABLE_ANALYSIS_ENABLED)
    target_compile_definitions(computer_player PUBLIC TRANSPOSITION_TABLE_ANALYSIS_ENABLED=1)
endif()

if(QUIESCENT_SEARCH_ENABLED)
    target_compile_definitions(computer_player PUBLIC QUIESCENT_SEARCH_ENABLED=1)
endif()

if(INCREMENTAL_STATIC_EVALUATION_VALIDATION_ENABLED)
    target_compile_definitions(computer_player PUBLIC INCREMENTAL_STATIC_EVALUATION_VALIDATION_ENABLED=1)
endif()
