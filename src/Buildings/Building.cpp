#include "Building.h"

int BuildingIndexToTeam(int index) {
    auto buildingIndex = static_cast<BuildingIndex>(index);
    switch (buildingIndex) {
        case BuildingIndex::CITY_GREEN:
        case BuildingIndex::ALT_HQ_GREEN:
        case BuildingIndex::HQ_GREEN:
        case BuildingIndex::FACTORY_GREEN:
        case BuildingIndex::AIRPORT_GREEN:
        case BuildingIndex::PORT_GREEN:
        case BuildingIndex::ALT_PORT_GREEN:
        case BuildingIndex::ROCKET_GREEN:
            return 0;
        case BuildingIndex::CITY_BLUE:
        case BuildingIndex::ALT_HQ_BLUE:
        case BuildingIndex::HQ_BLUE:
        case BuildingIndex::FACTORY_BLUE:
        case BuildingIndex::AIRPORT_BLUE:
        case BuildingIndex::PORT_BLUE:
        case BuildingIndex::ALT_PORT_BLUE:
        case BuildingIndex::ROCKET_BLUE:
            return 1;
        case BuildingIndex::CITY_RED:
        case BuildingIndex::ALT_HQ_RED:
        case BuildingIndex::HQ_RED:
        case BuildingIndex::FACTORY_RED:
        case BuildingIndex::AIRPORT_RED:
        case BuildingIndex::PORT_RED:
        case BuildingIndex::ALT_PORT_RED:
        case BuildingIndex::ROCKET_RED:
            return 2;
        case BuildingIndex::CITY_YELLOW:
        case BuildingIndex::ALT_HQ_YELLOW:
        case BuildingIndex::HQ_YELLOW:
        case BuildingIndex::FACTORY_YELLOW:
        case BuildingIndex::AIRPORT_YELLOW:
        case BuildingIndex::PORT_YELLOW:
        case BuildingIndex::ALT_PORT_YELLOW:
        case BuildingIndex::ROCKET_YELLOW:
            return 3;
        case BuildingIndex::CITY_GREY:
        case BuildingIndex::ALT_HQ_GREY:
        case BuildingIndex::HQ_GREY:
        case BuildingIndex::FACTORY_GREY:
        case BuildingIndex::AIRPORT_GREY:
        case BuildingIndex::PORT_GREY:
        case BuildingIndex::ALT_PORT_GREY:
        case BuildingIndex::ROCKET_GREY:
            return 4;
        default:
            return -1;
    }
}

Building::Building(int tileIndex, TileType type, SDL_Point coordinates) {
    _tileIndex = tileIndex;
    _type = type;
    _coordinates = coordinates;
    _capturing = nullptr;
    _team = BuildingIndexToTeam(tileIndex);
}

std::unique_ptr<Building> BuildingFactory::createBuilding(TileType type, int tileIndex, SDL_Point coordinates) {
    switch (type) {
        case TileType::HQ:
            return std::make_unique<HQ>(tileIndex, type, coordinates);
        case TileType::CITY:
            return std::make_unique<City>(tileIndex, type, coordinates);
        case TileType::FACTORY:
            return std::make_unique<Factory>(tileIndex, type, coordinates);
        case TileType::AIRPORT:
            return std::make_unique<Airport>(tileIndex, type, coordinates);
        case TileType::PORT:
            return std::make_unique<Port>(tileIndex, type, coordinates);
        default:
            return nullptr;
    }
}


