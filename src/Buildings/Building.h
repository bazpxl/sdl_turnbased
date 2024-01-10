#ifndef SDL_BASEGAME_BUILDING_H
#define SDL_BASEGAME_BUILDING_H


#include "Units/Unit.h"

class Building {
public:
    Building(int tileIndex, TileType type, SDL_Point coordinates);
    int getTeam();
private:
    Unit *_capturing;
    [[maybe_unused]] int _captureProgess = 20;
    int _team;
    int _tileIndex;
    TileType _type;
    SDL_Point _coordinates{};
};

class HQ : public Building {
public:
    HQ(int tileIndex, TileType type, SDL_Point coordinates) : Building(tileIndex, type, coordinates) {}
};

class City : public Building {
public:
    City(int tileIndex, TileType type, SDL_Point coordinates) : Building(tileIndex, type, coordinates) {}
};

class Factory : public Building {
public:
    Factory(int tileIndex, TileType type, SDL_Point coordinates) : Building(tileIndex, type, coordinates) {}
};

class Airport : public Building {
public:
    Airport(int tileIndex, TileType type, SDL_Point coordinates) : Building(tileIndex, type, coordinates) {}
};

class Port : public Building {
public:
    Port(int tileIndex, TileType type, SDL_Point coordinates) : Building(tileIndex, type, coordinates) {}
};

class BuildingFactory {
public:
    static std::unique_ptr<Building> createBuilding(TileType type, int tileIndex, SDL_Point coordinates);
};



#endif //SDL_BASEGAME_BUILDING_H
