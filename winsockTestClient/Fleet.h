
#ifndef WINSOCKTESTCLIENT_FLEET_H
#define WINSOCKTESTCLIENT_FLEET_H


#include <stdio.h>
#include <vector>
#include <string>
#include <map>

class Fleet {
public:
    std::map<std::string,int> SHIPS = {
            {"Carrier",5},
            {"Battleship",4},
            {"Cruiser",3},
            {"Destroyer1",2},{"Destroyer2",2},
            {"Submarine1",1},{"Submarine2",1}
    };

    char shipNames[7][25] = {"Carrier","Battleship","Cruiser","Destroyer1","Destroyer2","Submarine1","Submarine2"};
};


#endif //WINSOCKTESTCLIENT_FLEET_H
