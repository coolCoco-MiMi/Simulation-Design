cmake_minimum_required(VERSION 3.19)
project(CPUChipSim)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPUChipSim main.cpp Wafer.cpp Wafer.h)