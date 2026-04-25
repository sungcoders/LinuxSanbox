cmake_minimum_required(VERSION 3.22)
project(udpclient)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    client.cpp 
)

add_executable(udpclient ${SOURCES})