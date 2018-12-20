//
// Created by ivand on 12/20/2018.
//

#include "Game2maybe.h"

#include <iostream>
#include <ctime>

void Game2maybe::clear(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            map[i][j] = 0;
            enemyMap[i][j]= 0;
        }
    }
}

void Game2maybe::show() {
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Game2maybe::showEnemyMap() {
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            std::cout << enemyMap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Game2maybe::ships(){
    int s = 0;
    while(s < maxShips){
        int x = rand() % rows;
        int y = rand() % col;
        if(map[x][y] !=1){
            s++;
            map[x][y] = 1;
        }

    }
}

bool Game2maybe::attack(int x, int y){
    if(map[x][y] == 1){
        map[x][y] = 2;
        return true;
    }
    return false;
}
int Game2maybe::numberOfShips(){

    int c = 0;

    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            if (map[i][j] == 1){
                c++;
            }
        }
    }
    return c;
}

void Game2maybe::run(){
    srand(time(NULL));
    clear();
    ships();

    int pos1;
    int pos2;

    char prompt;

    while(1){
        std::cout << "Please input a location: "; std::cin >> pos1 >> pos2;

        if(attack(pos1, pos2))
            std::cout << "You wasted a fool" << std::endl;

            //cout << "Do you want to continue? (y/n) "; cin >> prompt;
            //if(prompt == 'n')
            //   break;

        else
            std::cout << "No lucky for you, ploplo" << std::endl;

        std::cout << "Number of ships left " << numberOfShips() << std::endl;
        std::cout << "Do you want to continue? (y/n) "; std::cin >> prompt;
        if(prompt == 'n')
            break;


        if(numberOfShips()== 0)
            break;

    }
    std::cout << "Game Over bitch" << std::endl;
    system("pause");
}


