cmake_minimum_required(VERSION 3.0)
project(optionparser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DDEBUG)

include_directories(optionparser)

file(GLOB Sources optionparser/*.cc)
add_library(optionparser ${Sources})

project(opttest)
add_executable(opttest test.cc)
target_link_libraries(opttest optionparser)
