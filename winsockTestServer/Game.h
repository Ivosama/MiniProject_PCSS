//
// Created by pbusk on 17-12-2018.
//



#ifndef MINIPROJECT_PCSS_GAME_H
#define MINIPROJECT_PCSS_GAME_H


#include <stdio.h>
#include <vector>
#include <string>

class Game {
private:
    int p1Array[10][10];
    int p2Array[10][10];


    int getHit(int hitX, int hitY, int player); // player = 0 || player = 1
    void updateMap(int newArray[10][10], int player); // player = 0 || player = 1

public:
    void populateArrays();
    int resolveRound(const char* input);

};


#endif //MINIPROJECT_PCSS_GAME_H
