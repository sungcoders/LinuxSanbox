cmake_minimum_required(VERSION 3.22)
project(tcpclient)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    client.cpp 
)

add_executable(tcpclient ${SOURCES})