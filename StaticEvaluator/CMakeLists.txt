cmake_minimum_required (VERSION 3.8)

set(SOURCES
    StaticEvaluator.cpp
    StaticEvaluator.h
)

set(PRIVATE_INCLUDE_PATHS
    .
)

set(PUBLIC_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Misc/include
)

add_library(static_evaluator ${SOURCES})
target_include_directories(static_evaluator PRIVATE ${PRIVATE_INCLUDE_PATHS} PUBLIC ${PUBLIC_INCLUDE_PATHS})
