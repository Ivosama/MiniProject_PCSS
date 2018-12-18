//
// Created by pbusk on 17-12-2018.
//

#include "Game.h"

int p1Array[10][10];
int p2Array[10][10];

void populateArrays() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            p1Array[i][j] = 0;
            p2Array[i][j] = 0;
        }
    }
}

void updateMap(int newArray[], int player) {
    if (player == 0) {
        p1Array = newArray;
    } else {
        p2Array = newArray;
    }
}

int getHit(int hitX, int hitY, int player) {

    if (player == 0) {
        if (p1Array[hitX][hitY] == 0) {
            return 0;
        } else {
            return 1
        }
    } else {
        if (p2Array[hitX][hitY] == 0) {
            return 0;
        } else {
            return 1
        }
    }
}