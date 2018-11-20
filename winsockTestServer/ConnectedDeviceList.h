//
// Created by User on 6/11/2018.
//

#ifndef WINSOCKTESTSERVER_CONNECTEDDEVICELIST_H
#define WINSOCKTESTSERVER_CONNECTEDDEVICELIST_H

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include "DeviceConnected.h"

struct Node{
    DeviceConnected* connectedDevice;
    Node *link;
};

typedef Node* NodePtr;

class ConnectedDeviceList {
public:
    NodePtr linklist_ptr, tail_ptr, iter;
    static int size;

    void head_insert(SOCKET ClientSocket);

    void tail_insert(SOCKET ClientSocket);

    void printValues();

    void deleteListItem(int index);

    ConnectedDeviceList();

    void sendAllClients(char message[]);

private:
    void init_insert(SOCKET ClientSocket);
};


#endif //WINSOCKTESTSERVER_CONNECTEDDEVICELIST_H
