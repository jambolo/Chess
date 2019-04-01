cmake_minimum_required (VERSION 3.4)

set(SOURCES
    BitBoard.cpp
    BitBoard.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(BitBoard ${SOURCES})
target_include_directories(BitBoard
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)
target_link_libraries(BitBoard PUBLIC
    Chess
    Misc::Misc
)

source_group(Sources FILES ${SOURCES})

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif()
