
cmake_minimum_required(VERSION 3.15)

project(SLVPiOs CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(COMPILE_OPTIONS
#     -Wall
#     -Wextra
#     -Werror
#     -Wpedantic
# )

add_subdirectory(app)
# add_subdirectory(framework)
# add_subdirectory(lib)
add_subdirectory(utils)
