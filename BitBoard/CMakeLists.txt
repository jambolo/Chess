cmake_minimum_required(VERSION 3.21)

add_library(BitBoard)

set_target_properties(BitBoard PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_sources(BitBoard
    PRIVATE
        BitBoard.cpp
    PUBLIC
        FILE_SET HEADERS 
        BASE_DIRS ${CMAKE_SOURCE_DIR}
        FILES BitBoard.h
)

target_include_directories(BitBoard
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BitBoard 
    PUBLIC
    Misc::Misc
)

add_library(BitBoard::BitBoard ALIAS BitBoard)

# Source grouping for IDEs
get_target_property(BITBOARD_SOURCES BitBoard SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${BITBOARD_SOURCES})

#########################################################################
# Documentation                                                         #
#########################################################################

option(BITBOARD_BUILD_DOCS "Build BitBoard documentation" OFF)

if(BITBOARD_BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(BitBoard_docs 
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating BitBoard API documentation with Doxygen"
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
