#ifndef SDL_BASEGAME_ENUMS_H
#define SDL_BASEGAME_ENUMS_H

enum class BuildingIndex{
    CITY_GREY = 8,
    CITY_GREEN = 40,
    CITY_BLUE = 72,
    CITY_RED = 104,
    CITY_YELLOW = 136,
    ALT_HQ_GREY = 9,
    ALT_HQ_GREEN = 41,
    ALT_HQ_BLUE = 73,
    ALT_HQ_RED = 105,
    ALT_HQ_YELLOW = 137,
    HQ_GREY = 10,
    HQ_GREEN = 42,
    HQ_BLUE = 74,
    HQ_RED = 106,
    HQ_YELLOW = 138,
    FACTORY_GREY = 11,
    FACTORY_GREEN = 43,
    FACTORY_BLUE = 75,
    FACTORY_RED = 107,
    FACTORY_YELLOW = 139,
    PORT_GREY = 12,
    PORT_GREEN = 44,
    PORT_BLUE = 76,
    PORT_RED = 108,
    PORT_YELLOW = 140,
    ALT_PORT_GREY = 13,
    ALT_PORT_GREEN = 45,
    ALT_PORT_BLUE = 77,
    ALT_PORT_RED = 109,
    ALT_PORT_YELLOW = 141,
    ROCKET_GREY = 14,
    ROCKET_GREEN = 46,
    ROCKET_BLUE = 78,
    ROCKET_RED = 110,
    ROCKET_YELLOW = 142,
    AIRPORT_GREY = 15,
    AIRPORT_GREEN = 47,
    AIRPORT_BLUE = 79,
    AIRPORT_RED = 111,
    AIRPORT_YELLOW = 143
};

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
