cmake_minimum_required(VERSION 3.0)
project(OptionParser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

file(GLOB Sources optionparser/*.cc)
add_library(OptionParser ${Sources})
target_include_directories(OptionParser PUBLIC optionparser)

project(opttest)
add_executable(opttest test.cc)
target_link_libraries(opttest OptionParser)
