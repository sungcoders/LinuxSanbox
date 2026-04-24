
#  Copyright (c) 2026-present, Sunglv. All rights reserved.
#  Author: Sunglv
#  This source code is licensed under the MIT license that can be found in the LICENSE file.
#  Project: slv

cmake_minimum_required(VERSION 3.15)
project(SLV)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(hello)
add_subdirectory(sample)
add_subdirectory(playback)
add_subdirectory(socket)

# cmd build debug
# cmake -DCMAKE_BUILD_TYPE=Debug ..
