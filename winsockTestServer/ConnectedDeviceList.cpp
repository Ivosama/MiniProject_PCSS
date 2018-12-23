//
// Created by User on 6/11/2018.
//

#include "ConnectedDeviceList.h"
int ConnectedDeviceList::size = 0;

using namespace std;
ConnectedDeviceList::ConnectedDeviceList() {
    // initialize init size
    size = 0;
}


// function definitions
void ConnectedDeviceList::head_insert(SOCKET ClientSocket){
    if (size == 0) {
        init_insert(ClientSocket);
    } else {
        NodePtr temp_ptr;
        temp_ptr = new Node;

        //DeviceConnected* connectedDevice =  connectedDevice(ClientSocket);

        temp_ptr->connectedDevice = new DeviceConnected(ClientSocket);
        temp_ptr->link = linklist_ptr;

        linklist_ptr = temp_ptr;
    }
    size++;
}

void ConnectedDeviceList::tail_insert(SOCKET ClientSocket){
    cout << size << endl;
    if (size == 0) {
        init_insert(ClientSocket);
    } else {
        NodePtr tail;
        NodePtr temp_ptr;
        temp_ptr = new Node;

        DeviceConnected connectedDevice(ClientSocket);

        temp_ptr->connectedDevice = new DeviceConnected(ClientSocket);
        temp_ptr->link = NULL;

        for(iter=linklist_ptr; iter!=NULL; iter=iter->link){
            tail = iter;
        }

        tail->link = temp_ptr;
        tail_ptr = temp_ptr;
    }
    size++;
}

void ConnectedDeviceList::deleteListItem(int index) {
    if (size > 1) {
        int i = 0;
        NodePtr lastPointer;
        for (iter=linklist_ptr; iter!=NULL; iter=iter->link) {
            if (i == index) {
                if (i == 0) {
                    // If your deleting the first one
                    linklist_ptr = iter->link;
                    linklist_ptr->connectedDevice->deviceNumber = 0;
                } else {
                    // If your deleting any other than the first one
                    NodePtr nextPointer = iter->link;
                    //nextPointer->connectedDevice->deviceNumber = nextPointer->connectedDevice->deviceNumber-1;
                    iter->link = nextPointer;
                    printValues();
                }
                size--;
                break;
            }
            lastPointer = iter;
            i++;
        }
    } else {
        cout << "Tried to delete node when the size was only 1 or less" << endl;
    }
}

void ConnectedDeviceList::printValues() {
    // start reading entries from the head of the linked list
    for(iter=linklist_ptr; iter!=NULL; iter=iter->link){
        cout << iter->link << endl;
    }
}

void ConnectedDeviceList::init_insert(SOCKET ClientSocket) {
    linklist_ptr = new Node;
    tail_ptr=linklist_ptr;
    DeviceConnected connectedDevice(ClientSocket);
    linklist_ptr->connectedDevice = new DeviceConnected(ClientSocket);
    linklist_ptr->link = NULL;
}
/*
 * Commented out as it gave some compiler errors, might be fixable
void ConnectedDeviceList::sendAllClients(char message[]) {
    if (sizeof(message) > DEFAULT_BUFLEN) {
        cout << "Message too long to send to all" << endl;
    } else {
        // start reading entries from the head of the linked list
        for(iter=linklist_ptr; iter!=NULL; iter=iter->link){
            iter->connectedDevice->sendMessageClient(message);
        }
    }
} */