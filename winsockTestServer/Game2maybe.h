//
// Created by ivand on 12/20/2018.
//

#ifndef MINIPROJECT_PCSS_GAME2MAYBE_H
#define MINIPROJECT_PCSS_GAME2MAYBE_H

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
    void run(char input[]);
};


#endif //WINSOCKTESTSERVER_GAME2MAYBE_H
