cmake_minimum_required(VERSION 3.15)
project(Hello)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    hello.cpp 
)

add_executable(hello ${SOURCES})