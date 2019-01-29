cmake_minimum_required (VERSION 3.4)

include(GoogleTest)

add_definitions(
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
    -DVC_EXTRALEAN
    -D_CRT_SECURE_NO_WARNINGS
    -D_SECURE_SCL=0
    -D_SCL_SECURE_NO_WARNINGS
)

include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)

file(GLOB FILES *.cpp)
foreach(FILE ${FILES})
    get_filename_component(TEST ${FILE} NAME_WE)
    add_executable(Chess_${TEST} ${FILE})
    target_link_libraries(Chess_${TEST} Chess gtest gtest_main)
    gtest_discover_tests(Chess_${TEST})
    set_target_properties(Chess_${TEST} PROPERTIES FOLDER tests)
    target_compile_features(Chess_${TEST} PUBLIC cxx_std_17)
    set_target_properties(Chess_${TEST} PROPERTIES CXX_EXTENSIONS OFF)
endforeach()
