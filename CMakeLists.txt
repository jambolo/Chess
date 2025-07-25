cmake_minimum_required (VERSION 3.10)
project(ChessPlayer VERSION 1.0.0 LANGUAGES CXX)


option(BUILD_SHARED_LIBS "Build libraries as DLLs" FALSE)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_package(nlohmann_json REQUIRED)
    find_package(Misc REQUIRED)
#    find_package(GamePlayer REQUIRED)
endif()

set(CHESS_PLAYER_INCLUDE_PATHS
    .
)

set(CHESS_PLAYER_SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    main.cpp
    Player.cpp
    Player.h
)
source_group(Sources FILES ${CHESS_PLAYER_SOURCES})

add_executable(chess_player ${CHESS_PLAYER_SOURCES})
target_include_directories(chess_player PRIVATE ${CHESS_PLAYER_INCLUDE_PATHS})
target_link_libraries(chess_player PUBLIC
    Chess
    GamePlayer
    nlohmann_json::nlohmann_json
)
target_compile_definitions(chess_player
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)

#configure_file("${PROJECT_SOURCE_DIR}/Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")

#########################################################################
# Testing                                                               #
#########################################################################

include(CTest)
enable_testing()
message(STATUS "Testing is enabled. Turn on BUILD_TESTING to build tests.")

#########################################################################
# Libraries                                                             #
#########################################################################

add_subdirectory(BitBoard)
add_subdirectory(Chess)
add_subdirectory(GamePlayer)
add_subdirectory(OpeningProcessor)
add_subdirectory(ZHash)
