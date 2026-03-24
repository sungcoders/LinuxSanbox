cmake_minimum_required(VERSION 3.15)

add_library(utils SHARED
    src/Log.cpp
)

# Export header
target_include_directories(utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set_target_properties(utils PROPERTIES
    OUTPUT_NAME "utils"
)

target_link_libraries(utils
    PUBLIC
        fmt
)
