//
// Created by pbusk on 17-12-2018.
//



#ifndef MINIPROJECT_PCSS_GAME_H
#define MINIPROJECT_PCSS_GAME_H


#include <stdio.h>
#include <vector>
#include <string>

class Game {
public:
    int p1Array[10][10];
    int p2Array[10][10];

    void populateArrays();
    int getHit(int hitX, int hitY, int player); // player = 0 || player = 1
    void updateMap(int newArray[], int player); // player = 0 || player = 1
    int resolveRound(const char* input);

};


#endif //MINIPROJECT_PCSS_GAME_H
