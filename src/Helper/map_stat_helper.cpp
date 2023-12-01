#include "map_stat_helper.h"

MapStats::MapStats(std::vector<std::vector<std::vector<int>>>* map) : _map(map) {
    _movementCosts = {
            {TileType::ROAD,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}}},

            {TileType::PLAIN,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 2},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}}},

            {TileType::FOREST,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 3},
                            {MovementType::TIRE_B, 3},
                            {MovementType::TREAD, 2},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}}},

            {TileType::MOUNTAIN,
                    {
                            {MovementType::INFANTRY, 2},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, std::numeric_limits<int>::max()},
                            {MovementType::TIRE_B, std::numeric_limits<int>::max()},
                            {MovementType::TREAD, std::numeric_limits<int>::max()},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            },

            {TileType::RIVER,
                    {
                            {MovementType::INFANTRY, 2},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, std::numeric_limits<int>::max()},
                            {MovementType::TIRE_B, std::numeric_limits<int>::max()},
                            {MovementType::TREAD, std::numeric_limits<int>::max()},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            },

            {TileType::LANDBRIDGE,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            },

            {TileType::SEABRIDGE,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, 1},
                            {MovementType::TRANSPORT_BOAT, 1}
                    }
            },

            {TileType::BEACH,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, 0},
                            {MovementType::TRANSPORT_BOAT, 1}
                    }
            },

            {TileType::SEA,
                    {
                            {MovementType::INFANTRY, std::numeric_limits<int>::max()},
                            {MovementType::MECH, std::numeric_limits<int>::max()},
                            {MovementType::TIRE_A, std::numeric_limits<int>::max()},
                            {MovementType::TIRE_B, std::numeric_limits<int>::max()},
                            {MovementType::TREAD, std::numeric_limits<int>::max()},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, 1},
                            {MovementType::TRANSPORT_BOAT, 1}
                    }
            },

            {TileType::CITY,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            },

            {TileType::FACTORY,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            },

            {TileType::AIRPORT,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            },

            {TileType::PORT,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, 1},
                            {MovementType::TRANSPORT_BOAT, 1}
                    }
            },

            {TileType::HQ,
                    {
                            {MovementType::INFANTRY, 1},
                            {MovementType::MECH, 1},
                            {MovementType::TIRE_A, 1},
                            {MovementType::TIRE_B, 1},
                            {MovementType::TREAD, 1},
                            {MovementType::AIR, 1},
                            {MovementType::SEA, std::numeric_limits<int>::max()},
                            {MovementType::TRANSPORT_BOAT, std::numeric_limits<int>::max()}
                    }
            }
    };

    _defense = {
            {TileType::ROAD,       0},
            {TileType::PLAIN,      1},
            {TileType::FOREST,     3},
            {TileType::MOUNTAIN,   4},
            {TileType::RIVER,      0},
            {TileType::LANDBRIDGE, 0},
            {TileType::SEABRIDGE,  0},
            {TileType::BEACH,      0},
            {TileType::SEA,        0},
            {TileType::CITY,       2},
            {TileType::FACTORY,    3},
            {TileType::AIRPORT,    3},
            {TileType::PORT,       3},
            {TileType::HQ,         4}
    };
    _indexTileTypeMapping = {
            {0,   TileType::PLAIN},
            {1,   TileType::PLAIN},
            {2,   TileType::PLAIN},
            {3,   TileType::RIVER},
            {4,   TileType::RIVER},
            {5,   TileType::MOUNTAIN},
            {18,  TileType::BEACH},
            {19,  TileType::BEACH},
            {20,  TileType::BEACH},
            {21,  TileType::RIVER},
            {22,  TileType::RIVER},
            {36,  TileType::BEACH},
            {37,  TileType::SEA},
            {38,  TileType::BEACH},
            {39,  TileType::SEA}, // TODO: Check if this is correct
            {54,  TileType::BEACH},
            {55,  TileType::BEACH},
            {56,  TileType::BEACH},
            {57,  TileType::RIVER},
            {72,  TileType::SEA}, // TODO: Check if this is correct
            {73,  TileType::BEACH}, // TODO: Check if this is correct
            {74,  TileType::SEA}, // TODO: Check if this is correct
            {75,  TileType::SEA}, // TODO: Check if this is correct
            {90,  TileType::SEA}, // TODO: Check if this is correct
            {91,  TileType::SEA}, // TODO: Check if this is correct
            {92,  TileType::SEA}, // TODO: Check if this is correct
            {93,  TileType::SEA}, // TODO: Check if this is correct
            {94,  TileType::FOREST},
            {108, TileType::ROAD},
            {109, TileType::ROAD},
            {110, TileType::ROAD},
            {111, TileType::ROAD},
            {112, TileType::FOREST},
            {126, TileType::ROAD},
            {127, TileType::ROAD},
            {128, TileType::ROAD},
            {129, TileType::ROAD},
            {130, TileType::BRIDGE},
            {144, TileType::ROAD},
            {145, TileType::ROAD},
            {146, TileType::ROAD},
            {147, TileType::ROAD},
            {148, TileType::SEA},
            {162, TileType::ROAD},
            {163, TileType::ROAD},
            {164, TileType::ROAD},
            {165, TileType::ROAD},
            {166, TileType::BRIDGE},
            {8,   TileType::CITY},
            {26,  TileType::CITY},
            {44,  TileType::CITY},
            {62,  TileType::CITY},
            {80,  TileType::CITY},
            {9,   TileType::HQ},
            {27,  TileType::HQ},
            {45,  TileType::HQ},
            {63,  TileType::HQ},
            {81,  TileType::HQ},
            {11,  TileType::FACTORY},
            {29,  TileType::FACTORY},
            {47,  TileType::FACTORY},
            {65,  TileType::FACTORY},
            {83,  TileType::FACTORY},
            {12,  TileType::FACTORY},
            {30,  TileType::FACTORY},
            {48,  TileType::FACTORY},
            {66,  TileType::FACTORY},
            {84,  TileType::FACTORY},
            {15,  TileType::FACTORY},
            {33,  TileType::FACTORY},
            {51,  TileType::FACTORY},
            {69,  TileType::FACTORY},
            {87,  TileType::FACTORY}
    };
}

MapStats &MapStats::getInstance(std::vector<std::vector<std::vector<int>>> *map) {
    static MapStats instance(map);
    return instance;
}

TileType MapStats::getTileType(int x, int y) const {

    if (y < 0 || y >= int((*_map)[0].size())) {
        return TileType::UNKNOWN;
    }

    if (x < 0 || x >= int((*_map)[0][y].size())) {
        return TileType::UNKNOWN;
    }

    int tileId = (*_map)[1][y][x] == -1 ? (*_map)[0][y][x] : (*_map)[1][y][x];
    auto tileTypeIter = _indexTileTypeMapping.find(tileId);

    if (tileTypeIter == _indexTileTypeMapping.end()) {
        return TileType::UNKNOWN;
    }

    TileType tileType = tileTypeIter->second;

    if (tileType == TileType::BRIDGE) {
        TileType baseTileType = _indexTileTypeMapping.at((*_map)[0][y][x]);
        return baseTileType == TileType::SEA ? TileType::SEABRIDGE : TileType::LANDBRIDGE;
    }

    return tileType;
}


int MapStats::getMovementCost(int x, int y, MovementType movementType) const {

    TileType tileType = getTileType(x, y);
    return _movementCosts.at(tileType).at(movementType);
}

int MapStats::getDefense(int x, int y) const {
    TileType tileType = getTileType(x, y);
    return _defense.at(tileType);
}
