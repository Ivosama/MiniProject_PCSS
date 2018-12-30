cmake_minimum_required(VERSION 3.10)
project(winsockTestServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(winsockTestServer
        ConnectedDeviceList.cpp
        ConnectedDeviceList.h
        DeviceConnected.cpp
        DeviceConnected.h
        Game.cpp
        Game.h
        Game2maybe.cpp
        Game2maybe.h
        main.cpp)
