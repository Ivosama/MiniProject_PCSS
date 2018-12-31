cmake_minimum_required(VERSION 3.10)
project(winsockTestClient)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(SOURCE_FILES main.cpp)

add_executable(winsockTestClient
        main.cpp
        clientGame.cpp clientGame.h Fleet.cpp Fleet.h)
