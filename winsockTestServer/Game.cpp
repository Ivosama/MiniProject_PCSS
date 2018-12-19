//
// Created by pbusk on 17-12-2018.
//

#include "Game.h"

#include <stdio.h>
#include <vector>

int p1Array[10][10];
int p2Array[10][10];

void Game::populateArrays() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            p1Array[i][j] = 0;
            p2Array[i][j] = 0;
        }
    }
}

void Game::updateMap(int newArray[10][10], int player) {
    if (player == 0) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                p1Array[i][j] = newArray[i][j];
            }
        }
    } else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                p2Array[i][j] = newArray[i][j];
            }
        }
    }
}

int Game::resolveRound(const char* input) {

    std::string str = (std::string) input;
    std::vector<char> data(str.begin(), str.end());
    if (data.size() > 4) {
        int posX = (int) data[0];
        int posY = (int) data[2];
        int player = (int) data[4];

        return (getHit(posX, posY, player));

    } else {
        return 0;
    }
}

int Game::getHit(int hitX, int hitY, int player) {

    if (player == 0) {
        if (p1Array[hitX][hitY] == 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (p2Array[hitX][hitY] == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}