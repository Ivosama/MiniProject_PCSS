//#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT
#define _WIN32_WINNT 0x501

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

int __cdecl main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    //char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    argv[1] = "127.0.0.1";

    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            std::printf("Invalid socket");
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            std::printf("Socket error");
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    // If there is no socket,
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        std::printf("Invalid socket");
        return 1;
    }

    // Loop for sending and receiving data
    char input[512];    // declares how big the input can be in chars
    do{
        char input[512];
        cout<< "Enter a message to send to server: (max 512 characters)\n";
        cin.getline(input,sizeof(input));
        //char tab2[1024];
        //strcpy(tab2, sendString.c_str());

        // The user input is sent to the server here
        iResult = send( ConnectSocket, input, (int)sizeof(input), 0 );

        // Check whether the socket is valid, otherwise, close it
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        // If the user inputs exit, shut down the program/ leave the server
        if (!strcmp(input, "exit")) {
            break;
        }

        //cout << strcmp(input, "exit") << endl;
        //printf("Bytes Sent: %ld\n", iResult);

        // Print the received data
        recv(ConnectSocket, recvbuf, recvbuflen, 0);
        cout << recvbuf << endl;
    } while(1);

    // Troubleshooting, shows whether the loop is still going, should only EVER show up if user writes "exit"
    cout << "send/receive ended" << endl;

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    /*
     * Old receive do-while loop - was never reached, so moved receive to the send do-while loop
     *
    // Receive until the peer closes the connection
    cout << "just before the do-while receiving" << endl;
    do {
        cout << "receiving message" << endl;
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        cout << "message received" << endl;
        if (!strcmp(recvbuf, "exit")) break;
        cout << recvbuf << "first thing" << endl;
        if (!strncmp(recvbuf, "--[reply]--", 4)) {
            cout << recvbuf << "second thing" << endl;
        }

        if ( iResult > 0 ){
            break;
        }
            //printf("Bytes received: %d\n", iResult);]
        else if ( recvbuf == 0 )
            printf("Connection closed\n");
        else
            // Disconnected from server lata biiiitch
            break;
            //printf("recv failed with error: %d\n", WSAGetLastError());

    //} while( iResult > 0 );
    } while(1);

    Was supposed to print the stuff which was received, now redundant
    string recString(recvbuf);
    cout<<recString<<endl;

    cout << iResult << endl;
     */

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

