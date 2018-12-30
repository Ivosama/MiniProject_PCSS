cmake_minimum_required(VERSION 3.13)
project(winsockTestClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(winsockTestClient
        main.cpp
        clientGame.cpp clientGame.h Fleet.cpp Fleet.h)
