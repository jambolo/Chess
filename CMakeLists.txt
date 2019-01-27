cmake_minimum_required (VERSION 3.4)
project(ChessPlayer CXX)

option(BUILD_SHARED_LIBS       "Build libraries as DLLs"                    FALSE)

#set(Chess_VERSION_MAJOR 0)
#set(Chess_VERSION_MINOR 1)
#configure_file("${PROJECT_SOURCE_DIR}/Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")

set(CHESS_PLAYER_SOURCES
    ComputerPlayer.cpp
    ComputerPlayer.h
    main.cpp
    Player.cpp
    Player.h
)

set(CHESS_PLAYER_INCLUDE_PATHS
    .
)

add_executable(chess_player ${CHESS_PLAYER_SOURCES})
target_link_libraries(chess_player
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
target_include_directories(chess_player PRIVATE ${CHESS_PLAYER_INCLUDE_PATHS})

source_group(Sources FILES ${CHESS_PLAYER_SOURCES})

add_subdirectory(BitBoard)
add_subdirectory(Chess)
add_subdirectory(GamePlayer)
add_subdirectory(json)
add_subdirectory(Misc)
add_subdirectory(OpeningProcessor)
add_subdirectory(ZHash)
