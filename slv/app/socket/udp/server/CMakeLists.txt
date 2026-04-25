cmake_minimum_required(VERSION 3.22)
project(udpserver)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    server.cpp 
)

add_executable(udpserver ${SOURCES})