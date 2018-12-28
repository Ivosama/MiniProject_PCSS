#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


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
    ZeroMemory(recvbuf, recvbuflen);
    //argv[1] = "127.0.0.1";

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
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    printf("Connected!");

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    char input[512];
    do{
        // Prompt the user for some text // USER ALWAYS HAS TO INPUT SOMETHING BEFORE RECEIVING ANYTHING // THIS SHOULD BE FIXED
        cout << "> " << endl;
        cin.getline(input,sizeof(input));

        if (!strcmp(input, "exit")) {
            break;
        } else {
            // Send the text
            int sendResult = send(ConnectSocket, input, 512, 0);
            if (sendResult != SOCKET_ERROR)
            {
                // Wait for response
                ZeroMemory(input, 4096);
                int bytesReceived = recv(ConnectSocket, input, 512, 0);
                if (bytesReceived != SOCKET_ERROR)
                {
                    // display response in console
                    cout << "SERVER> " << string(input, 0, bytesReceived) << endl;
                } else {
                    // Print error code
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ConnectSocket);
                    WSACleanup();
                    return 1;
                }
            } else {
                // Print error code
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
        }
    } while(1);


    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (!strcmp(recvbuf, "exit")) break;
        cout << "yikers" << recvbuf << endl;
        if (!strncmp(recvbuf, "--[reply]--", 4)) {
            cout << "wtf" << recvbuf << endl;
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

    string recString(recvbuf);
    cout<<"yikers at the bottom" << recString<<endl;

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

