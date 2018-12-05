cmake_minimum_required (VERSION 3.4)

set(SOURCES
    Player.cpp
    Player.h
)

set(PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(player ${SOURCES})
target_include_directories(player PRIVATE ${PRIVATE_INCLUDE_PATHS})
target_link_libraries(player game_state zhash misc)

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
