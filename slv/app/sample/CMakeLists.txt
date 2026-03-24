cmake_minimum_required(VERSION 3.15)
project(Sample)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    main.cpp
    src/sample.cpp
)

add_executable(sample ${SOURCES})

target_include_directories(sample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(sample
    PRIVATE
        utils
)
