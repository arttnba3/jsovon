# project test
cmake_minimum_required(VERSION 3.30)

# basic requirement
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# for debug only
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O0 -Wall")

# temporarily we need this for `import std`, as it's still experimental
# hopt that it could be removed soon in the future
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
set(CMAKE_CXX_MODULE_STD 1)

# test project
project(test_parser LANGUAGES CXX)
add_subdirectory(jsovon)
add_executable(test test.cpp)
target_link_libraries(test PUBLIC JsOvOn)
