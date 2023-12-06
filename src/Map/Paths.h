//
// Created by Jonas Luven on 05.12.23.
//

#ifndef SDL_BASEGAME_PATHS_H
#define SDL_BASEGAME_PATHS_H

#include <vector>
#include <SDL_rect.h>
#include "Helper/map_stat_helper.h"
#include <queue>
#include <iostream>
#include <SDL.h>
#include "Helper/render_singleton.h"
#include "global.h"


class Paths {
public:
    struct Node {
        Node(SDL_Point point, SDL_Point parent, int cost, int totalCost) : _coordinates(point),
                                                                           _parentCoordinates(parent), _cost(cost),
                                                                           _totalCost(totalCost) {};

        bool operator>(const Node &other) const { return _totalCost > other._totalCost; }

        SDL_Point _coordinates;
        SDL_Point _parentCoordinates;
        int _cost;
        int _totalCost;
    };

    std::vector<SDL_Point> getMoveRadius(SDL_Point start, MovementType movementType, int actionPoints);

    std::vector<SDL_Point> getPath(SDL_Point start, SDL_Point end, MovementType movementType, int actionPoints);

    bool mouseInRadius(SDL_Point pos);

    //std::vector<SDL_Point> getAttackRadius();
    Paths(const std::vector<std::vector<std::vector<int>>> &map, const MapStats &mapStats);

    void drawMoveRadius(u32 frame);

private:
    const MapStats &_mapStats;

    const std::vector<std::vector<std::vector<int>>> &_map;

    std::vector<std::vector<std::vector<int>>> _weightedGraphs;

    std::vector<Node> _cachedMoveRadius;


    u32 _offset = 0;

    std::vector<std::vector<int>> convertToWeightedGraph(MovementType movementType);

    [[nodiscard]] const std::vector<std::vector<int>> &getWeightedGraph(MovementType movementType) const;

    std::vector<Node> getNeighbors(Node *node, const std::vector<std::vector<int>> &weightedGraph);

    bool validPoint(const SDL_Point &point);

    static Node *pointInVector(const SDL_Point &point, std::vector<Node> &vector);

    static std::vector<SDL_Point> nodeToPointVector(std::vector<Node> &vector);

};


#endif //SDL_BASEGAME_PATHS_H
