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
    ROCKETS,
    ANTI_AIR,
    MISSILES,
    B_COPTER,
    T_COPTER,
    FIGHTER,
    BOMBER,
    BATTLESHIP,
    CRUISER,
    LANDER,
    SUBMARINE
};

enum class TileType {
    ROAD,
    PLAIN,
    FOREST,
    MOUNTAIN,
    RIVER,
    LANDBRIDGE,
    SEABRIDGE,
    BEACH,
    SEA,
    CITY,
    FACTORY,
    AIRPORT,
    PORT,
    HQ
};
#endif //SDL_BASEGAME_ENUMS_H
