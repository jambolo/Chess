cmake_minimum_required (VERSION 3.4)

set(PROCESS_SOURCES
    main.cpp
)

set(ECO_INCLUDE_PATHS
    .
)

add_executable(process ${PROCESS_SOURCES})
target_link_libraries(process
    game_state
    nlohmann_json::nlohmann_json
)
target_compile_definitions(process
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)
target_include_directories(process PRIVATE ${PROCESS_INCLUDE_PATHS})
