#ifndef SDL_BASEGAME_MAP_STAT_HELPER_H
#define SDL_BASEGAME_MAP_STAT_HELPER_H

#include <map>
#include "enums.h"

class MapStats {
public:
    static MapStats &getInstance();

    int getMovementCost(TileType tileType, MovementType movementType);

    int getDefense(TileType tileType);

private:
    MapStats();

    std::map<TileType, std::map<MovementType, int>> _movementCosts;
    std::map<TileType, int> _defense;
};


#endif //SDL_BASEGAME_MAP_STAT_HELPER_H
