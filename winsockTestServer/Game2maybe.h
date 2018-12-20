//
// Created by ivand on 12/20/2018.
//

#ifndef WINSOCKTESTSERVER_GAME2MAYBE_H
#define WINSOCKTESTSERVER_GAME2MAYBE_H

#include <iostream>
#include <ctime>


class Game2maybe {
public:
    const int rows = 10;
    const int col = 10;

    int maxShips = 10;

    int map[10][10];
    int enemyMap[10][10];

    void clear();
    void show();
    void showEnemyMap();
    void ships();
    bool attack(int x, int y);
    int numberOfShips();
    void run();
};


#endif //WINSOCKTESTSERVER_GAME2MAYBE_H
