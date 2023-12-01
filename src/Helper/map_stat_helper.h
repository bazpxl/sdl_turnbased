#ifndef SDL_BASEGAME_MAP_STAT_HELPER_H
#define SDL_BASEGAME_MAP_STAT_HELPER_H

#include <map>
#include "enums.h"

class MapStats {
public:
    static MapStats &getInstance(std::vector<std::vector<std::vector<int>>> *map);

    [[nodiscard]] TileType getTileType(int x, int y) const;

    [[nodiscard]] int getMovementCost(int x, int y, MovementType movementType) const;

    [[nodiscard]] int getDefense(int x, int y) const;

private:
    explicit MapStats(std::vector<std::vector<std::vector<int>>> *map);

    std::vector<std::vector<std::vector<int>>> *_map;
    std::unordered_map<TileType, std::map<MovementType, int>> _movementCosts;
    std::unordered_map<TileType, int> _defense;
    std::unordered_map<int, TileType> _indexTileTypeMapping;
};


#endif //SDL_BASEGAME_MAP_STAT_HELPER_H
