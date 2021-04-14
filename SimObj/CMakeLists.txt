cmake_minimum_required(VERSION 3.19)
project(SimObj)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SimObj
        Entity.cpp
        Entity.h
        Event.cpp
        Event.h
        Queue.cpp
        Queue.h
        SimObj.cpp
        SimObj.h
        Main.cpp)
