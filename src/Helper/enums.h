#ifndef SDL_BASEGAME_ENUMS_H
#define SDL_BASEGAME_ENUMS_H

enum class MovementType {
    INFANTRY,
    MECH,
    TIRE_A,
    TIRE_B,
    TREAD,
    AIR,
    SEA,
    TRANSPORT_BOAT
};

enum class UnitType {
    INFANTRY,
    MECH,
    RECON,
    TANK,
    MEDIUM_TANK,
    APC,
    ARTILLERY,
    ROCKET,
    ANTI_AIR,
    MISSILE,
    BATTLE_COPTER,
    TRANSPORT_COPTER,
    FIGHTER,
    BOMBER,
    LANDER,
    CRUISER,
    SUBMARINE,
    BATTLESHIP
};

enum class TileType {
    ROAD,
    PLAIN,
    FOREST,
    MOUNTAIN,
    RIVER,
    BRIDGE,
    LANDBRIDGE,
    SEABRIDGE,
    BEACH,
    SEA,
    CITY,
    FACTORY,
    AIRPORT,
    PORT,
    HQ,
    UNKNOWN
};

enum class Team {
    GREEN,
    BLUE,
    RED,
    YELLOW,
    NEUTRAL
};

// Arrow Type
enum class AT {
    LEFT_ARROW = 0,
    RIGHT_ARROW = 1,
    UP_ARROW = 2,
    DOWN_ARROW = 3,
    HOR_END_LEFT = 4,
    HOR_END_RIGHT = 5,
    VER_END_UP = 6,
    VER_END_DOWN    = 7,
    HOR_CONNECT = 8,
    VER_CONNECT = 9,
    // judging by spritesheet orientation
    UP_RIGHT_CONNECT    = 10,
    UP_LEFT_CONNECT    = 11,
    DOWN_RIGHT_CONNECT  = 12,
    DOWN_LEFT_CONNECT   = 13,
    // custom end pieces
    VERT_UP_RIGHT_END   = 14,
    VERT_UP_LEFT_END    = 15,
    VERT_DOWN_RIGHT_END = 16,
    VERT_DOWN_LEFT_END  = 17,
    HOR_UP_RIGHT_END    = 18,
    HOR_UP_LEFT_END    = 19,
    HOR_DOWN_RIGHT_END  = 20,
    HOR_DOWN_LEFT_END   = 21,
};
#endif //SDL_BASEGAME_ENUMS_H
