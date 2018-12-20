cmake_minimum_required(VERSION 3.12)
project(winsockTestServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(winsockTestServer
        main.cpp DeviceConnected.cpp DeviceConnected.h ConnectedDeviceList.cpp ConnectedDeviceList.h Game.cpp Game.h Game2maybe.cpp Game2maybe.h)



if(WIN32)
    target_link_libraries(winsockTestServer wsock32 ws2_32)
endif()