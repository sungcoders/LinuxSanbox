cmake_minimum_required(VERSION 3.15)

add_library(utils SHARED
    src/UtilsLog.cpp
    src/UtilsEvent.cpp
    src/UtilsTick.cpp
)

# Export header
target_include_directories(utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(utils
    PUBLIC
        fmt
)
