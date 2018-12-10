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
option(FEATURE_PLAYER_ANALYSIS "ComputerPlayer analysis is enabled if true" FALSE)


#set(Chess_VERSION_MAJOR 0)
#set(Chess_VERSION_MINOR 1)
#configure_file("${PROJECT_SOURCE_DIR}/Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")

set(CHESS_SOURCES
    Chess.cpp
)

set(CHESS_PRIVATE_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_executable(chess ${CHESS_SOURCES})
target_link_libraries(chess
    computer_player
    game_state
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
target_include_directories(chess PRIVATE ${CHESS_PRIVATE_INCLUDE_PATHS})

if(FEATURE_PLAYER_ANALYSIS)
    target_compile_definitions(chess PUBLIC FEATURE_PLAYER_ANALYSIS=1)
endif()

add_subdirectory(BitBoard)
add_subdirectory(ComputerPlayer)
add_subdirectory(GameState)
add_subdirectory(Misc)
add_subdirectory(Player)
add_subdirectory(ZHash)
