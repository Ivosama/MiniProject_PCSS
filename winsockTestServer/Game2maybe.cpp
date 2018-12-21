//
// Created by ivand on 12/20/2018.
//

#include "Game2maybe.h"

#include <iostream>
#include <ctime>

//clears the map arrays and sets them to 0
void Game2maybe::clear(){
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            map[i][j] = 0;
            enemyMap[i][j]= 0;
        }
    }
}
//Shows the map
void Game2maybe::show() {
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
//Shows the enemy map
void Game2maybe::showEnemyMap() {
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<col; j++){
            std::cout << enemyMap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
//Sets the ships to positions (random)
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
//This is how you attack, in this way 1 1 for example
bool Game2maybe::attack(int x, int y){
    if(map[x][y] == 1){
        map[x][y] = 2;
        return true;
    }
    return false;
}
//Returns the number of ships left
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


//Runs the game, the whole thing, takes receive buff as input and transform it into integers to put in the attack function
//Need to transform those char into integers, I think now we are getting ASCII values which would be 49 for 1, and that is not valid.
void Game2maybe::run(char input[]){
    srand(time(NULL));
    clear();
    ships();

    int pos1 = input[0] - 48;
    int pos2 = input[2] - 48;

    //char prompt;

    while(1){
        //std::cout << "Please input a location: "; std::cin >> pos1 >> pos2;

        if(attack(pos1, pos2))
            std::cout << "You wasted a fool" << std::endl;

            //cout << "Do you want to continue? (y/n) "; cin >> prompt;
            //if(prompt == 'n')
            //   break;

        else
            std::cout << "No lucky for you, ploplo" << std::endl;

        std::cout << "Number of ships left " << numberOfShips() << std::endl;
        //std::cout << "Do you want to continue? (y/n) "; std::cin >> prompt;
        //if(prompt == 'n')
            //break;


        if(numberOfShips()== 0)
            break;

    }
    std::cout << "Game Over bitch" << std::endl;
    system("pause");
}


