//
// Created by User on 6/11/2018.
//

#ifndef WINSOCKTESTSERVER_DEVICECONNECTED_H
#define WINSOCKTESTSERVER_DEVICECONNECTED_H

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

class DeviceConnected {
public:

    int deviceNumber;
    HANDLE hThread;
    SOCKET ClientSocket = INVALID_SOCKET;
    DeviceConnected();
    DeviceConnected(SOCKET _ClientSocket);
    void clear();
    void show();
    void sendMessageClient(char message[]);
    //void ClientSession(SOCKET ClientSocket);
    //unsigned __stdcall ClientSession(void *data);
};

#endif //WINSOCKTESTSERVER_DEVICECONNECTED_H
