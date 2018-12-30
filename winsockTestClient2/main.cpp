#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include "clientGame.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;
clientGame game;

int main()
{
    string ipAddress = "127.0.0.1";			// IP Address of the server
    int port = 27015;						// Listening port # on the server

    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock, Err #" << wsResult << endl;
        return 1;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Fill hint struct
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    char buf[4096];
    string userInput;
    // Do-while loop to send and receive data


    ZeroMemory(buf, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived > 0)
    {
        // display response in console
        cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
    }

    {
        //Initial setup, clear and generate map
        std::cout << "Randomly generated ships" << std::endl;
        game.clearMaps();
        srand(time(0)); //seeds random function
        std::cout << "Do you want to place ships manually (type m) or randomly (type r)?" << std::endl;
        while(!game.okay) {
            cin >> game.input;
            if(strlen(game.input) == 1) {    //make sure input is 1 character
                if (game.input[0] == 82 || game.input[0] == 114) {    //if R or r
                    std::cout << "Spawning randomly" << std::endl;
                    game.spawnShips(0);
                    game.okay = true;
                } else if (game.input[0] == 77 || game.input[0] == 109) {   //if M or m
                    std::cout << "Pretending to place ships manually" << std::endl;
                    game.spawnShips(0);//put manual ship placement function here
                    game.okay = true;
                }
            }
            if(!game.okay){  //if invalid input
                std::cout << "Invalid input. Please enter m or r" << std::endl;
            }
        }
        game.spawnShips(1);  //spawns enemy ships. Replace with messages from server
        //spawnEasy(1); //spawns one 2-length ship for testing
        game.compressMap(game.myMap);
        game.showEnemyMap();
        game.showMyMap();

        char compAdded[101];
        compAdded[0] = 'm';
        for (int i = 1; i < 101; i++) {
            compAdded[i] = game.comp[i-1];
        }

        int sendResult = send(sock, compAdded, 101, 0);
        if (sendResult != SOCKET_ERROR){
            cout << "Tried to send, no error" << endl;
        }
        //int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        ZeroMemory(buf, 4096);
        cout << "Waiting to receive..." << endl;
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived > 0)
        {
            // display response in console
            cout << "SERVER thing up top> " << string(buf, 0, bytesReceived) << endl;
            char justEnemyMap[100];
            memcpy(justEnemyMap, buf, 100);
            game.expandMap(justEnemyMap);

            //game.expandMap(buf);
        }

        game.showEnemyMap();
        // PROBABLY WANT TO SEND myMAP HERE, I IMAGINE //
        //showEnemyMapDebug();  //shows enemy ship positions
        /*do{ //REPLACE 5000 with the enemy player's roll, both here and below.
            game.roll = game.rollOrder();
            //send this player's roll here
            //receive enemy player's roll here
            if (game.roll > 5000) {
                game.myTurn = true;
                std::cout << "You go first" << std::endl;
            } else {
                game.myTurn = false;
                std::cout << "You go second" << std::endl;
            }
        }while(game.roll == 5000);*/
        //actual game loop
        do{ //while the whole thing is running
            while(game.myTurn) {    //while it's this client's turn
                std::cout << "Enter co-ordinates to attack" << std::endl;
                std::cout << "Please write it like number,number (i.e 3,6)" << std::endl;
                do { //continues to ask for inputs until you give a valid one
                    cin >> game.input;
                    if (game.checkValidInput(game.input)) {
                        game.okay = true;
                        std::cout << "Detected valid input: " << game.input << std::endl;
                        game.firstNum = static_cast<int>(game.input[0]);
                        game.secondNum = static_cast<int>(game.input[2]);
                        send(sock, game.input, sizeof(game.input),0);
                    } else {
                        game.okay = false;
                        std::cout << "Your input was shitty, try again: " << game.input << std::endl;
                    }
                } while (!game.okay);
                game.myTurn = !game.attackTarget(game.firstNum - 48, game.secondNum - 48, 0);  //if valid shot, returns true (myTurn false)
                game.showEnemyMap();
                //showEnemyMapDebug();
                if(game.checkVictory()){
                    std::cout << "You win" << std::endl;
                    //it should send a message to server here, saying the game is over and it won
                    game.running = false;
                }
            }
            if(game.myTurn == true) {
                std::cout << "turn ending" << std::endl;
                std::cout << "Info to send to server: " << game.firstNum - '0' << "," << game.secondNum - '0'
                          << std::endl;
                // SEND THE ENTERED INPUT TO SERVER
                int sendResult = send(sock, game.input, sizeof(game.input), 0);
            }
            //showEnemyMap();
            //system("pause");    //Replace this with server message to say when it's your turn. First turn should be randomly picked by server.
            //attackTarget(4,4,1);  //Replace this with the message from enemy saying where they shot. Only happens if the enemy did a turn (sometimes this player goes first). Can assume it's a valid location because enemy client is checking that.
            ZeroMemory(buf, 4096);
            char bytesReceived = recv(sock, buf, 4096, 0);
            if (bytesReceived > 0)
            {
                // display response in console
                cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                game.attackTarget(buf[10]-'0',buf[12]-'0',1);
                //game.expandMap(buf);
            }
            game.myTurn = true;
        }while(game.running);
        // Prompt the user for some text // USER ALWAYS HAS TO INPUT SOMETHING BEFORE RECEIVING ANYTHING // THIS SHOULD BE FIXED
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0)		// Make sure the user has typed in something
        {
            // Send the text
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                // Wait for response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    // display response in console
                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }

    } while (userInput.size() > 0);

    //  close down everything
    closesocket(sock);
    WSACleanup();
    return 0;
}