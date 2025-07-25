cmake_minimum_required (VERSION 3.10)

set(PROCESS_ECO_SOURCES
    main.cpp
)

set(PROCESS_ECO_INCLUDE_PATHS
    .
)

add_executable(process_eco ${PROCESS_ECO_SOURCES})
target_link_libraries(process_eco PUBLIC
    Chess
    nlohmann_json::nlohmann_json
)
target_compile_definitions(process_eco
    PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DVC_EXTRALEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SECURE_SCL=0
        -D_SCL_SECURE_NO_WARNINGS
)
target_include_directories(process_eco PRIVATE ${PROCESS_ECO_INCLUDE_PATHS})
source_group(Sources FILES ${PROCESS_ECO_SOURCES})
