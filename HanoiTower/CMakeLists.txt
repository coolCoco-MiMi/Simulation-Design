cmake_minimum_required(VERSION 3.19)
project(Hanoi_Tower)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Hanoi_Tower
        Cube.cpp
        Cube.h
        Game.cpp
        Game.h
        HSLAPixel.cpp
        HSLAPixel.h
        main.cpp
        Stack.cpp
        Stack.h)
