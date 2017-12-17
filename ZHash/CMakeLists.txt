cmake_minimum_required (VERSION 3.4)

set(SOURCES
    ZHash.cpp
    ZHash.h
    ZHash.inl
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Misc/include
)

add_library(zhash ${SOURCES})
target_include_directories(zhash PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})
target_link_libraries(zhash misc)