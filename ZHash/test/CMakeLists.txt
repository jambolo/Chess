cmake_minimum_required (VERSION 3.10)

set(GTEST_ROOT "" CACHE PATH "Location of GTest")
find_package(GTest REQUIRED)
include(GoogleTest)

add_definitions(
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
    -DVC_EXTRALEAN
    -D_CRT_SECURE_NO_WARNINGS
    -D_SECURE_SCL=0
    -D_SCL_SECURE_NO_WARNINGS
)

set(SOURCES
    test-ZHash.cpp
)

message(STATUS "Building tests for ZHash")

foreach(FILE ${SOURCES})
    get_filename_component(TEST ${FILE} NAME_WE)
    set(TEST_EXE "ZHash_${TEST}")
    add_executable(${TEST_EXE} ${FILE})
    message(STATUS "Adding test executable: ${TEST_EXE}")
    target_link_libraries(${TEST_EXE} PRIVATE ZHash GTest::GTest GTest::Main)
    gtest_discover_tests(${TEST_EXE})
    target_compile_features(${TEST_EXE} PRIVATE cxx_std_17)
    set_target_properties(${TEST_EXE} PROPERTIES CXX_EXTENSIONS OFF)
endforeach()
