//
// Created by User on 6/11/2018.
//

#include "DeviceConnected.h"
#include "ConnectedDeviceList.h"

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

DeviceConnected::DeviceConnected(SOCKET _ClientSocket) {
    ClientSocket = _ClientSocket;
    deviceNumber = ConnectedDeviceList::size;
    //ClientSession(ClientSocket);
    //unsigned threadID = ConnectedDeviceList::size;
    //hThread = (HANDLE)DeviceConnected::_beginthreadex(NULL, 0, &DeviceConnected::ClientSession, (void*)ClientSocket, 0, &threadID);
}