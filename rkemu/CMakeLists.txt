cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -g -static")

project(rkemu CXX)

add_executable(rkemu main.cpp debug.cpp cpu.cpp ../utils/utils.cpp)
