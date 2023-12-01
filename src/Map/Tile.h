#ifndef SDL_BASEGAME_TILE_H
#define SDL_BASEGAME_TILE_H

#include <map>
#include "Units/Unit.h"



class Tile {
public:
    explicit Tile(TileType type) : _type(type) {}

    [[nodiscard]] TileType getType() const {
        return _type;
    }

private:
    const TileType _type;
};



#endif //SDL_BASEGAME_TILE_H
