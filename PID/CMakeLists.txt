cmake_minimum_required(VERSION 3.19)
project(PID)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PID
        ControllerState.cpp
        ControllerState.h
        EulersMethod.cpp
        EulersMethod.h
        Parameters.h
        PID.cpp)
