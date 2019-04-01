cmake_minimum_required (VERSION 3.4)

set(SOURCES
    ZHash.cpp
    ZHash.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
)

add_library(ZHash ${SOURCES})
target_include_directories(ZHash
    PUBLIC ${PUBLIC_INCLUDE_PATHS}
    PRIVATE ${PRIVATE_INCLUDE_PATHS}
)

target_link_libraries(ZHash Chess Misc::Misc)
source_group(Sources FILES ${SOURCES})

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif()
