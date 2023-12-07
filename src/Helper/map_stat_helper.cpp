#include <limits>
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

            {32,  TileType::BEACH},
            {33,  TileType::BEACH},
            {34,  TileType::BEACH},
            {35,  TileType::RIVER},
            {36,  TileType::RIVER},
            {64,  TileType::BEACH},
            {65,  TileType::SEA},
            {66,  TileType::BEACH},
            {67,  TileType::SEA}, // TODO: Check if this is correct
            {96,  TileType::BEACH},
            {97,  TileType::BEACH},
            {98,  TileType::BEACH},
            {99,  TileType::RIVER},
            {128,  TileType::SEA}, // TODO: Check if this is correct
            {129,  TileType::BEACH}, // TODO: Check if this is correct
            {130,  TileType::SEA}, // TODO: Check if this is correct
            {131,  TileType::SEA}, // TODO: Check if this is correct
            {160,  TileType::SEA}, // TODO: Check if this is correct
            {161,  TileType::SEA}, // TODO: Check if this is correct
            {162,  TileType::SEA}, // TODO: Check if this is correct
            {163,  TileType::SEA}, // TODO: Check if this is correct
            {164,  TileType::FOREST},
            {192, TileType::ROAD},
            {193, TileType::ROAD},
            {194, TileType::ROAD},
            {195, TileType::ROAD},
            {196, TileType::FOREST},
            {224, TileType::ROAD},
            {225, TileType::ROAD},
            {226, TileType::ROAD},
            {227, TileType::ROAD},
            {228, TileType::BRIDGE},
            {256, TileType::ROAD},
            {257, TileType::ROAD},
            {258, TileType::ROAD},
            {259, TileType::ROAD},
            {260, TileType::SEA},
            {288, TileType::ROAD},
            {289, TileType::ROAD},
            {290, TileType::ROAD},
            {291, TileType::ROAD},
            {292, TileType::BRIDGE},
            {8,   TileType::CITY},
            {40,  TileType::CITY},
            {72,  TileType::CITY},
            {104,  TileType::CITY},
            {136,  TileType::CITY},
            {9,   TileType::HQ},
            {10,  TileType::HQ},
            {41,  TileType::HQ},
            {42,  TileType::HQ},
            {73,  TileType::HQ},
            {74,   TileType::HQ},
            {105,  TileType::HQ},
            {106,  TileType::HQ},
            {137,  TileType::HQ},
            {138,  TileType::HQ},
            {11,  TileType::FACTORY},
            {43,  TileType::FACTORY},
            {75,  TileType::FACTORY},
            {107,  TileType::FACTORY},
            {139,  TileType::FACTORY},
            {15,  TileType::AIRPORT},
            {47,  TileType::AIRPORT},
            {79,  TileType::AIRPORT},
            {111,  TileType::AIRPORT},
            {143,  TileType::AIRPORT},
            {12,  TileType::PORT},
            {44,  TileType::PORT},
            {76,  TileType::PORT},
            {108,  TileType::PORT},
            {140,  TileType::PORT},

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
