cmake_minimum_required(VERSION 3.22)
project(tcpserver)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    server.cpp 
)

add_executable(tcpserver ${SOURCES})