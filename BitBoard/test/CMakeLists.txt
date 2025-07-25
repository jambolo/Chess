cmake_minimum_required(VERSION 3.21)

find_package(GTest REQUIRED CONFIG)
include(GoogleTest)

# Function to create test executables
function(add_test test_name source_file)
    add_executable(${test_name} ${source_file})
    set_target_properties(${test_name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    target_link_libraries(${test_name} 
        PRIVATE 
            BitBoard::BitBoard
            GTest::gtest
            GTest::gtest_main
    )
    gtest_discover_tests(${test_name})
    
    message(STATUS "Added test executable: ${test_name}")
endfunction()

set(SOURCES
    test-BitBoard.cpp
)

message(STATUS "Building tests for BitBoard")

foreach(SOURCE ${SOURCES})
    get_filename_component(TEST_NAME ${SOURCE} NAME_WE)
    add_test("BitBoard_${TEST_NAME}" ${SOURCE})
endforeach()
