cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE "RelWithDebInfo")
# set(CMAKE_BUILD_TYPE "Debug")

project(
  AOC2025
  VERSION 0.10
  DESCRIPTION "Advent of Code 2025"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

add_executable(AOC main.cc)

target_compile_options(AOC PRIVATE -fconstexpr-steps=10271242)
target_compile_options(AOC PRIVATE -fconstexpr-depth=27)

add_subdirectory(../cpputil ${CMAKE_CURRENT_BINARY_DIR}/cpputil)

target_link_libraries(AOC cpputil)
