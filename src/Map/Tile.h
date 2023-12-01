#ifndef SDL_BASEGAME_TILE_H
#define SDL_BASEGAME_TILE_H

#include <map>
#include "Units/Unit.h"



class Tile {
private:
    const TileType _type;
    //const int _x, _y;


public:
    Tile(TileType type);
    int getMovementCost(UnitType unitType);
};


#endif //SDL_BASEGAME_TILE_H
