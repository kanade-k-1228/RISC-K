cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -g -static")

project(rkcc CXX)

add_executable(rkcc main.cpp token.cpp ast.cpp ../utils/utils.cpp)
