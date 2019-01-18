cmake_minimum_required(VERSION 2.8)
project(project2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

add_executable(project2 project2.cpp Task.cpp Task.h CPU.cpp CPU.h IO.cpp IO.h PQ.cpp PQ.h main.cpp PQ1.cpp PQ1.h)
