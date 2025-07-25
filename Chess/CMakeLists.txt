cmake_minimum_required(VERSION 3.21)

# Feature options
option(FEATURE_PRIORITIZED_MOVE_ORDERING "Enable prioritized move ordering" OFF)
option(FEATURE_INCREMENTAL_STATIC_EVALUATION "Enable incremental static evaluation" OFF)
option(FEATURE_BITBOARD_THREAT_DETECTION "Enable bitboard threat detection" OFF)
option(ANALYSIS_GAME_STATE "Enable general GameState analysis" OFF)

# Status messages for features
message(STATUS "Chess Library Configuration:")
message(STATUS "  FEATURE_PRIORITIZED_MOVE_ORDERING     : ${FEATURE_PRIORITIZED_MOVE_ORDERING}")
message(STATUS "  FEATURE_INCREMENTAL_STATIC_EVALUATION : ${FEATURE_INCREMENTAL_STATIC_EVALUATION}")
message(STATUS "  FEATURE_BITBOARD_THREAT_DETECTION     : ${FEATURE_BITBOARD_THREAT_DETECTION}")
message(STATUS "  ANALYSIS_GAME_STATE                   : ${ANALYSIS_GAME_STATE}")

add_library(Chess)

set_target_properties(Chess PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_sources(Chess
    PRIVATE
        Bishop.cpp
        Board.cpp
        GameState.cpp
        King.cpp
        Knight.cpp
        Move.cpp
        ResponseGenerator.cpp
        Pawn.cpp
        Piece.cpp
        Position.cpp
        Queen.cpp
        Rook.cpp
        StaticEvaluator.cpp
        ZHash.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}
        FILES
            Bishop.h
            Board.h
            GameState.h
            King.h
            Knight.h
            Move.h
            ResponseGenerator.h
            Pawn.h
            Piece.h
            Position.h
            Queen.h
            Rook.h
            StaticEvaluator.h
            Types.h
            ZHash.h
)

target_include_directories(Chess
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Chess 
    PUBLIC
        GamePlayer
        Misc::Misc
        nlohmann_json::nlohmann_json
)

# Configure feature-based compile definitions
if(FEATURE_PRIORITIZED_MOVE_ORDERING)
    target_compile_definitions(Chess PUBLIC FEATURE_PRIORITIZED_MOVE_ORDERING=1)
endif()

if(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    target_compile_definitions(Chess PUBLIC FEATURE_INCREMENTAL_STATIC_EVALUATION=1)
endif()

if(FEATURE_BITBOARD_THREAT_DETECTION)
    target_compile_definitions(Chess PUBLIC FEATURE_BITBOARD_THREAT_DETECTION=1)
endif()

if(ANALYSIS_GAME_STATE)
    target_compile_definitions(Chess PUBLIC ANALYSIS_GAME_STATE=1)
endif()

add_library(Chess::Chess ALIAS Chess)

# Source grouping for IDEs
get_target_property(CHESS_SOURCES Chess SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CHESS_SOURCES})

#########################################################################
# Documentation                                                         #
#########################################################################

option(CHESS_BUILD_DOCS "Build Chess documentation" OFF)

if(CHESS_BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(Chess_docs 
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating Chess API documentation with Doxygen"
            VERBATIM
            SOURCES ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        )
    endif()
endif()

#########################################################################
# Testing                                                               #
#########################################################################

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
