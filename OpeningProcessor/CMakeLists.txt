cmake_minimum_required(VERSION 3.21)

add_executable(process_eco)

set_target_properties(process_eco PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_sources(process_eco
    PRIVATE
        main.cpp
)

target_include_directories(process_eco
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(process_eco
    PRIVATE
        Chess::Chess
        nlohmann_json::nlohmann_json
)

# Source grouping for IDEs
get_target_property(PROCESS_ECO_SOURCES process_eco SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROCESS_ECO_SOURCES})
