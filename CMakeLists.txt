cmake_minimum_required (VERSION 3.4)
project(Chess CXX)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(chess_doxygen ALL
        ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        SOURCES Doxyfile
    )
endif(DOXYGEN_FOUND)

option(BUILD_SHARED_LIBS       "Build libraries as DLLs"                    FALSE)
option(PLAYER_ANALYSIS_ENABLED "ComputerPlayer analysis is enabled if true" FALSE)


#set(Chess_VERSION_MAJOR 0)
#set(Chess_VERSION_MINOR 1)
#configure_file("${PROJECT_SOURCE_DIR}/Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")

set(SOURCES
    Chess.cpp
    Player.cpp
    Player.h
)

add_executable(chess ${SOURCES})
target_link_libraries(chess
    bitboard
    game_state
    misc
    computer_player
    zhash
)
target_compile_definitions(chess
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)

if(PLAYER_ANALYSIS_ENABLED)
    target_compile_definitions(chess PUBLIC PLAYER_ANALYSIS_ENABLED=1)
endif()

add_subdirectory(BitBoard)
add_subdirectory(GameState)
add_subdirectory(Misc)
add_subdirectory(ComputerPlayer)
add_subdirectory(ZHash)
