#include <iostream>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <chrono>
#include <thread>
#include "clientGame.h"
#include "Fleet.h"

using namespace std;
bool myTurn = false;
bool running = true;
bool okay = false;
int myMap[10][10];
int tempMap[10][10];
int enemyMap[10][10];
int rows = 10;
int col = 10;
char input[20];
char comp[100]; //holds myMap in char array format so it's easier to send
int firstNum = 0;
int secondNum = 0;
int roll = 0;
char enemyMove[20];

int clientGame::rollOrder(){
    return rand() % 10000;
}

//turns myMap into a 100-long char array
void clientGame::compressMap(int map[10][10]){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            comp[i*10+j] = myMap[i][j];
        }
    }
}

//turns the char array back (from enemy) to a 10x10 int array for enemy map
void clientGame::expandMap(char temp[100]){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            enemyMap[i][j] = temp[i*10+j];
        }
    }
}

//Replaces temp map with my map
void clientGame::revertTemp(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            tempMap[i][j] = myMap[i][j];
        }
    }
}

//replaces my map with temp map
void clientGame::finalizeShip(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            myMap[i][j] = tempMap[i][j];
        }
    }
}

//temporary functions for generating enemy maps
void clientGame::finalizeShipEnemy(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            enemyMap[i][j] = tempMap[i][j];
        }
    }
}
void clientGame::revertTempEnemy(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            tempMap[i][j] = enemyMap[i][j];
        }
    }
}

//checks if given value is between 0 and 9 (in range of the map arrays)
bool clientGame::inRange(int temp){
    if(temp <= rows-1 && temp >= 0){
        return true;
    }
    else{
        return false;
    }
}

//Returns -1 or +1, to be used twice when generating ships (left/right, up/down)
int clientGame::randomDirection(){
    //srand(time(0));
    int temp = rand() % 2;  //randoms 0 or 1
    temp = temp * 2;    //doubles. 0 to 0, 1 to 2
    temp = temp - 1;    //minus one. 0 to -1, 2 to 1.
    return temp;    //returns -1 or +1
}

//places one ship on a map. length is length of the ship, p is which player's map to place it on (0 for p1, else p2)
void clientGame::placeOne(int length, int p){
    bool success = false;
    int nextShipSize = length-1;
    bool validPos = true;
    int hor, vert;
    //srand(time(0));
    do {    //repeats the following until it gets a valid position
        //srand(time(0));
        int x = rand() % 10;
        int y = rand() % 10;
        if(p == 0) {
            revertTemp();   //sets temp to current myMap
        }
        else{
            revertTempEnemy();
        }
        if (tempMap[x][y] != 1) {  //checks if start tile is taken
            int dir = randomDirection();    //see if horizontal (-1) or vertical (1)
            if(dir == -1){
                hor= randomDirection();
                vert = 0;
            }
            else{
                vert = randomDirection();
                hor = 0;
            }
            for (int i = 0; i <= nextShipSize; i++) {   //repeats for size of ship
                if(inRange(x + (hor * i)) && inRange(y + (vert * i)))   //checks to make sure the new tile is in bounds
                    validPos = true;
                else
                    validPos = false;
                if (tempMap[x + (hor * i)][y + (vert * i)] != 1 && validPos) {   //if next tile is empty + in bounds
                    tempMap[x + (hor * i)][y + (vert * i)] = 1;     //extends ship to there
                    //std::cout << (x + (hor * i)) << "," << (y + (vert * i)) <<std::endl;

                } else {
                    //revertTemp(); //otherwise undoes this ship progress to try again
                    i = nextShipSize + 1; //makes i too big so the for loop dies
                    success = false;    //makes fail true so the do-while continues
                }
                if (i == nextShipSize) { //if the for loop got this far (completed ship)
                    success = true;   //fail = false so it moves on
                }
            }
        }
    }while(!success);
    if(p == 0){
        finalizeShip(); //finalizes this ship placement by saving it to myMap
        std::cout << "Friendly ship created!" << std::endl;
    }
    else{
        finalizeShipEnemy(); //saves it to enemyMap instead
        std::cout << "Enemy ship created!" << std::endl;
    }
}

//Spawns ships randomly on the grid. 1x5, 2x4, 3x3
void clientGame::spawnShips(int p){
    placeOne(5,p);
    placeOne(4,p);
    placeOne(4,p);
    placeOne(3,p);
    placeOne(3,p);
    placeOne(3,p);
}

//spawns just one 2-length ship for testing
void clientGame::spawnEasy(int p){
    placeOne(2,p);
}

void clientGame::showMyMap() {
    std::cout << "Your map" << std::endl;
    std::cout << " " << " |" << " 0 1 2 3 4 5 6 7 8 9" << std::endl;
    std::cout << "  ---------------------" << std::endl;
    for (int i = 0; i < rows; i++) {
        std::cout << i << " |" << " ";
        for (int j = 0; j < col; j++) {
            if (myMap[i][j] == 0) {
                std::cout << "~" << " ";
            } else {
                char t = 241;
                std::cout << t << " ";
            }
        }
        std::cout << std::endl;
    }
}

//displays enemy map, with ships hidden
void clientGame::showEnemyMap(){
    std::cout << "Enemy map" << std::endl;
    std::cout << " " << " |" << " 0 1 2 3 4 5 6 7 8 9" << std::endl;
    std::cout << "  ---------------------" << std::endl;
    for (int i = 0; i<rows; i++){
        std::cout << i << " |" << " ";
        for (int j = 0; j<col; j++){
            if(enemyMap[i][j] == 0 || enemyMap[i][j] == 1)
                std::cout << "~" << " ";
            else
            if(enemyMap[i][j] == 2){
                std::cout << "*" << " ";
            }else
                std::cout << "X" << " ";

        }
        std::cout << std::endl;
    }
}

//displays enemy map with ships visible
void clientGame::showEnemyMapDebug(){
    std::cout << "Enemy map" << std::endl;
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            std::cout << enemyMap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void clientGame::clearMaps(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            myMap[i][j] = 0;
            enemyMap[i][j] = 0;
        }
    }
}
void clientGame::clearTempMap(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            tempMap[i][j] = 0;
        }
    }
}

//processes attack on target. p is player. 0 if client is shooting, else enemy is shooting
bool clientGame::attackTarget(int x, int y, int p) {
    if (p == 0) {
        std::cout << "Firing on co-ordinates " << x << ", " << y << std::endl;
        if (enemyMap[x][y] >= 2) {
            std::cout << "You've already shot there, try again..." << std::endl;
            return false;
        }
        if (enemyMap[x][y] == 0) {
            std::cout << "You missed!" << std::endl;
            enemyMap[x][y] = 2;
        }
        if (enemyMap[x][y] == 1) {
            std::cout << "Nice shot!" << std::endl;
            enemyMap[x][y] = 3;
        }
        //enemyMap[x][y] = 2;
        return true;
    }
    else{
        if (myMap[x][y] == 0) {
            std::cout << "The enemy has fired at " << x << "," << y  << " and missed!" << std::endl;
            myMap[x][y] = 2;
        }
        if (myMap[x][y] == 1) {
            std::cout << "The enemy has hit one of your ships at location: " << x << "," << y << std::endl;
            myMap[x][y] = 3;
        }
    }
}

//takes input and returns true if it's in format number,number
bool clientGame::checkValidInput(char in[]){
    if(strlen(input) == 3){
        //std::cout << "String is correct length" << std::endl;
        if (isdigit(in[0]) && isdigit(in[2])){  //Checks first and third chars to see if they're numbers
            //std::cout << "Correct format too" << std::endl;
            if(input[1] == 44) {    //makes sure middle char is a comma. We can change this later.
                //std::cout << "comma detect" << std::endl;
                return true;
            }
        }
    }
    return false;
}

//returns false if it finds any enemy ships remaining, else true
bool clientGame::checkVictory(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            if(enemyMap[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}

void clientGame::manualPlacement() {
    Fleet fleet;
    int totalShips = 0;
    while (totalShips < 7) {
        showMyMap();
        int size = fleet.SHIPS[fleet.shipNames[totalShips]];
        std::cout << "Place the ship Commander: " << fleet.shipNames[totalShips] << std::endl;
        std::cout << "Remember Commander! The ship is " << size << " spaces long!" << std::endl;
        char coord[20];
        std::cin >> coord;
        int pos[2] = {coord[0],coord[2]};
        if(coord[0]-48 < 0 || coord[0]-48 > 9){
            continue;
        }
        if(coord[2]-48 < 0 || coord[2]-48 > 9){
            continue;
        }
        std::cout << "Type H for Horizontal and V for vertical?" << std::endl;
        std::string hv;
        std::cin >> hv;

        if (hv[0] == 'V') {
            int check = 0;
            int initPos = pos[0]-48;
            int increasePos = pos[0] - 48;
            int otherPos = pos[1] - 48;
            while (check == 0) {
                if (myMap[increasePos++][otherPos] == 1) {
                    check++;
                }
                if (increasePos > initPos + size) {
                    break;
                }if(increasePos >= 9){
                    check++;
                }
            }
            if (check > 0) {
                std::cout << "The ship cannot fit in this space" << std::endl;
            } else {
                for (int x = initPos; x <= increasePos; x++) {
                    myMap[x][otherPos] = 1;
                }
                totalShips++;
                std::cout << "Ship is in position!" << std::endl;
            }
        }
        if (hv[0] == 'H') {
            int check = 0;
            int initPos = pos[1]-48;
            int increasePos = pos[1]-48;
            int otherPos = pos[0]-48;
            while (check == 0) {
                if (myMap[otherPos][increasePos++] == 1) {
                    check++;
                }
                if (increasePos > initPos + size) {
                    break;
                }
                if (increasePos >= 9) {
                    check++;

                }
            }
            if (check > 0) {
                std::cout << "The ship cannot fit in this space" << std::endl;
            } else {
                for (int x = initPos; x <= increasePos; x++) {
                    myMap[otherPos][x] = 1;
                }
                totalShips++;
                std::cout << "Ship is in position!" << std::endl;
            }
        }
    }
}

int clientGame::play() {
    //Initial setup, clear and generate map
    std::cout << "Randomly generated ships" << std::endl;
    clearMaps();
    srand(time(0)); //seeds random function
    std::cout << "Do you want to place ships manually (type m) or randomly (type r)?" << std::endl;
    while(!okay) {
        cin >> input;
        if(strlen(input) == 1) {    //make sure input is 1 character
            if (input[0] == 82 || input[0] == 114) {    //if R or r
                std::cout << "Spawning randomly" << std::endl;
                spawnShips(0);
                okay = true;
            } else if (input[0] == 77 || input[0] == 109) {   //if M or m
                std::cout << "Pretending to place ships manually" << std::endl;
                spawnShips(0);//put manual ship placement function here
                okay = true;
            }
        }
        if(!okay){  //if invalid input
            std::cout << "Invalid input. Please enter m or r" << std::endl;
        }
    }
    spawnShips(1);  //spawns enemy ships. Replace with messages from server
    //spawnEasy(1); //spawns one 2-length ship for testing
    showMyMap();
    showEnemyMap();
    //showEnemyMapDebug();  //shows enemy ship positions
    //actual game loop
    do{ //while the whole thing is running
        //showEnemyMap();
        system("pause");    //Replace this with server message to say when it's your turn. First turn should be randomly picked by server.
        //attackTarget(4,4,1);  //Replace this with the message from enemy saying where they shot. Only happens if the enemy did a turn (sometimes this player goes first). Can assume it's a valid location because enemy client is checking that.
        myTurn = true;
        while(myTurn) {    //while it's this client's turn
            std::cout << "Enter co-ordinates to attack" << std::endl;
            std::cout << "Do it like number,number or you're stupid" << std::endl;
            do { //continues to ask for inputs until you give a valid one
                cin >> input;
                if (checkValidInput(input)) {
                    okay = true;
                    std::cout << "Detected valid input: " << input << std::endl;
                    firstNum = static_cast<int>(input[0]);
                    secondNum = static_cast<int>(input[2]);
                } else {
                    okay = false;
                    std::cout << "Your input was shitty, try again: " << input << std::endl;
                }
            } while (!okay);
            myTurn = !attackTarget(firstNum - 48, secondNum - 48, 0);  //if valid shot, returns true (myTurn false)
            showEnemyMap();
            //showEnemyMapDebug();
            if(checkVictory()){
                std::cout << "You win" << std::endl;
                //it should send a message to server here, saying the game is over and it won
                running = false;
            }
        }
        std::cout << "turn ending" << std::endl;
        std::cout << "Info to send to server: " << firstNum-'0' << "," << secondNum-'0' << std::endl;
    }while(running);

    std::cout << "Shutting down" << std::endl;
    return 0;
}