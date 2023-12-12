#ifndef SDL_BASEGAME_MAP_STAT_HELPER_H
#define SDL_BASEGAME_MAP_STAT_HELPER_H

#include <map>
#include "enums.h"
#include "Units/Unit.h"
#include <vector>
#include <unordered_map>

class MapStats {
public:
    static MapStats &getInstance(std::vector<std::vector<std::vector<int>>> *map, std::vector<std::vector<Unit *>> *unitMap);

    [[nodiscard]] TileType getTileType(int x, int y) const;

    [[nodiscard]] int getMovementCost(int x, int y, MovementType movementType) const;

    [[nodiscard]] int getDefense(int x, int y) const;

    [[nodiscard]] int inUnitMapAndSameTeam(SDL_Point pos, SDL_Point start) const;

private:
    explicit MapStats(std::vector<std::vector<std::vector<int>>> *map, std::vector<std::vector<Unit *>> *unitMap);
    std::vector<std::vector<Unit *>> *_unitMap;
    std::vector<std::vector<std::vector<int>>> *_map;
    std::unordered_map<TileType, std::map<MovementType, int>> _movementCosts;
    std::unordered_map<TileType, int> _defense;
    std::unordered_map<int, TileType> _indexTileTypeMapping;
};


#endif //SDL_BASEGAME_MAP_STAT_HELPER_H
