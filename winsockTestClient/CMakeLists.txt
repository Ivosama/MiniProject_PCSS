cmake_minimum_required(VERSION 3.12)
project(winsockTestClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(winsockTestClient
        main.cpp)


if(WIN32)
    target_link_libraries(winsockTestClient wsock32 ws2_32)
endif()