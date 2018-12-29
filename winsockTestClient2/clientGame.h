//
// Created by PolleAnker on 26/12/2018.
//

#ifndef WINSOCKTESTCLIENT_CLIENTGAME_H
#define WINSOCKTESTCLIENT_CLIENTGAME_H

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <chrono>
#include <thread>

class clientGame{
public:
    bool myTurn = true;
    bool running = true;
    bool okay = false;
    int myMap[10][10];
    int tempMap[10][10];
    int enemyMap[10][10];
    int rows = 10;
    int col = 10;
    char input[20];
    char comp[100];
    int firstNum = 0;
    int secondNum = 0;
    int roll = 0;

    int rollOrder();
    void revertTemp();
    void compressMap(int map[10][10]);
    void expandMap(char temp[100]);
    void finalizeShip();
    void finalizeShipEnemy();
    void revertTempEnemy();
    bool inRange(int temp);
    int randomDirection();
    void placeOne(int length, int p);
    void spawnShips(int p);
    void spawnEasy(int p);
    void showMyMap();
    void showEnemyMap();
    void showEnemyMapDebug();
    void clearMaps();
    void clearTempMap();
    bool attackTarget(int x, int y, int p);
    bool checkValidInput(char in[]);
    bool checkVictory();
    int play();
};

#endif //WINSOCKTESTCLIENT_CLIENTGAME_H
