#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
    char playerOneMap[100];
    char playerTwoMap[100];
    int mapsLoaded = 0;

    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock! Quitting" << endl;
        return 1;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return 1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(27015);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Create the master file descriptor set and zero it
    fd_set master;
    FD_ZERO(&master);

    // Add the first socket as listening
    FD_SET(listening, &master);

    // this will be changed by the \quit command below, which can bbe sent from client to server - Remember the \

    bool running = true;

    while (running)
    {
        fd_set copy = master;

        //Amount of sockets
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		cout << "socketCount: " << socketCount << endl;
		cout << "Sizeof thing: " << (sizeof(copy.fd_array) / sizeof(copy.fd_array[0])) << endl;
        // Loop through all the current connections / potential connections
        for (int i = 0; i < master.fd_count; i++)
        {
			cout << i << endl;
            // Copy socket object from array
            SOCKET sock = copy.fd_array[i];

            // Is it an incoming connection?
            if (sock == listening)
            {
                // Accept a new connection
                SOCKET client = accept(listening, nullptr, nullptr);

                // Add the new connection to the list of connected clients
                FD_SET(client, &master);

                // Send a welcome message to the connected client
                string welcomeMsg = "Welcome to the server!\r\n";
                send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
            }
            else // It's an incoming message
            {
                char buf[4096];
                ZeroMemory(buf, 4096);

                // Receive message
                int bytesIn = recv(sock, buf, 4096, 0);
                cout << "Incoming message: " << buf << endl;

                if (bytesIn <= 0)
                {
                    // close socket if not receiving anything
                    printf("Dropping client");
                    closesocket(sock);
                    FD_CLR(sock, &master);
                }
                else
                {
                    // Check to see if it's a command. \quit kills the server
                    if (buf[0] == '\\')
                    {
                        // Is the command quit?
                        string cmd = string(buf, bytesIn);
                        std::cout << (cmd);
                        if (cmd == "\\quit")
                        {
                            running = false;
                            break;
                        }

                        // Put other commands here
                        continue;
                    }
                    string sendKey(1, buf[0]);
                    if (sendKey=="m") {
                        // It's a map!
                        if (i==0) {
                            std::cout << "Player One Map: " << endl;
                            for (int mapDupe = 1; mapDupe < 101; mapDupe++) {
                                playerOneMap[mapDupe-1] = buf[mapDupe];
                                std::cout << playerOneMap[mapDupe] << " ";
                            }
                        } else {
                            std::cout << "Player Two Map: " << endl;
                            for (int mapDupe = 1; mapDupe < 101; mapDupe++) {
                                playerTwoMap[mapDupe-1] = buf[mapDupe];
                                std::cout << playerTwoMap[mapDupe] << " ";
                            }
                        }
                        mapsLoaded++;
						if (mapsLoaded > 1) {
							/*
							for (int sendIter = 0; sendIter < master.fd_count; sendIter++)
							{
								SOCKET outSockALT = master.fd_array[sendIter];
								if (sendIter == 0) {
									send(outSockALT, playerTwoMap, 4096, 0);
								}
								else if (sendIter == 1) {
									send(outSockALT, playerOneMap, 4096, 0);
								}
							}
							*/
							int iSendResult = send(master.fd_array[1], playerOneMap, 4096, 0);

							if (iSendResult == SOCKET_ERROR)  {
								printf("send failed with error: %d\n", WSAGetLastError());
								closesocket(master.fd_array[0]);
								WSACleanup();
								return 1;
							}
							iSendResult = send(master.fd_array[2], playerTwoMap, 4096, 0);

							if (iSendResult == SOCKET_ERROR) {
								printf("send failed with error: %d\n", WSAGetLastError());
								closesocket(master.fd_array[1]);
								WSACleanup();
								return 1;
							}
							
							//send(master.fd_array[1], playerOneMap, 4096, 0);
						}
                    } else {
                        // Send message to other clients, and NOT the listening socket

                        for (int i = 0; i < master.fd_count; i++)
                        {
                            SOCKET outSock = master.fd_array[i];
                            if (outSock != listening && outSock != sock)
                            {
                                ostringstream ss;
                                ss << "Received: " << buf << "\r\n";
                                string strOut = ss.str();

                                send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                            }
                        }
                    }


                }
            }
        }
    }

    // Remove the listening socket from the master file descriptor and close it
    // to prevent anyone else trying to connect.
    FD_CLR(listening, &master);
    closesocket(listening);

    // Message to let users know if shutting down.
    string msg = "Server is shutting down. Goodbye\r\n";

    while (master.fd_count > 0)
    {
        // Get the socket number
        SOCKET sock = master.fd_array[0];

        // Send the goodbye message
        send(sock, msg.c_str(), msg.size() + 1, 0);

        // Remove it from the master file list and close the socket
        FD_CLR(sock, &master);
        closesocket(sock);
    }

    // Cleanup winsock
    WSACleanup();

    system("pause");
}