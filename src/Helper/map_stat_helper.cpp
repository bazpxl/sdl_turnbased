#include "map_stat_helper.h"

MapStats &MapStats::getInstance() {
    static MapStats instance;
    return instance;
}

int MapStats::getMovementCost(TileType tileType, MovementType movementType) {
    return _movementCosts.at(tileType).at(movementType);
}

int MapStats::getDefense(TileType tileType) {
    return _defense.at(tileType);
}

MapStats::MapStats() {
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
            {TileType::ROAD, 0},
            {TileType::PLAIN, 1},
            {TileType::FOREST, 3},
            {TileType::MOUNTAIN, 4},
            {TileType::RIVER, 0},
            {TileType::LANDBRIDGE, 0},
            {TileType::SEABRIDGE, 0},
            {TileType::BEACH, 0},
            {TileType::SEA, 0},
            {TileType::CITY, 2},
            {TileType::FACTORY, 3},
            {TileType::AIRPORT, 3},
            {TileType::PORT, 3},
            {TileType::HQ, 4}
    };
}
