cmake_minimum_required(VERSION 3.19)
project(CarFollowing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CarFollowing
        Car.cpp
        Car.h
        EulersMethod.cpp
        EulersMethod.h
        Main.cpp
        Road.cpp
        Road.h)
