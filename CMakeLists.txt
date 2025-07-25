cmake_minimum_required(VERSION 3.21)
project(ChessPlayer 
    VERSION 0.1.0 
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use modern CMAKE policies
cmake_policy(SET CMP0077 NEW)  # option() honors normal variables
cmake_policy(SET CMP0074 NEW)  # find_package uses <PackageName>_ROOT variables

# Project-wide options
option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# External dependencies
find_package(nlohmann_json REQUIRED)
find_package(Misc REQUIRED)
#find_package(GamePlayer REQUIRED)

if(WIN32)
    add_compile_definitions(
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        VC_EXTRALEAN
        _CRT_SECURE_NO_WARNINGS
        _SECURE_SCL=0
        _SCL_SECURE_NO_WARNINGS
)
endif()

set(CHESS_PLAYER_SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    main.cpp
    Player.cpp
    Player.h
)

add_executable(chess_player ${CHESS_PLAYER_SOURCES})
target_include_directories(chess_player PRIVATE .)
target_link_libraries(chess_player PRIVATE
    Chess
    GamePlayer
    nlohmann_json::nlohmann_json
)

# Source grouping for IDEs
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CHESS_PLAYER_SOURCES})

#########################################################################
# Testing                                                               #
#########################################################################

include(CTest)
if(BUILD_TESTING)
    enable_testing()
    message(STATUS "Testing is enabled.")
else()
    message(STATUS "Turn on BUILD_TESTING to build tests.")
endif()

#########################################################################
# Libraries                                                             #
#########################################################################

add_subdirectory(BitBoard)
add_subdirectory(Chess)
add_subdirectory(GamePlayer)
add_subdirectory(OpeningProcessor)
