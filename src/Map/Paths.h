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
#include "Helper/enums.h"


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
    struct SDLPointHash {
        std::size_t operator()(const SDL_Point &point) const {
            return std::hash<int>()(point.x) ^ (std::hash<int>()(point.y) << 1);
        }
    };

    struct SDLPointEqual {
        bool operator()(const SDL_Point &a, const SDL_Point &b) const {
            return a.x == b.x && a.y == b.y;
        }
    };

    std::vector<Node>
    getMoveRadius(SDL_Point start, MovementType movementType, int actionPoints, std::vector<Node> &radius);

    std::vector<SDL_Point>
    getPath(SDL_Point start, SDL_Point end, std::vector<Node> &radius);

    std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> getAttackRadius(SDL_Point start, int range, std::vector<Node> &radius);

    bool mouseInRadius(SDL_Point pos, std::vector<Node> &radius);

    Paths(const std::vector<std::vector<std::vector<int>>> &map, const MapStats &mapStats);

    void drawMoveRadius(u32 frame, std::vector<Node> &radius, std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> &attackRadius);

    void drawPath(std::vector<SDL_Point> &path);

    std::vector<int> _arrowPos;


    bool validPoint(const SDL_Point &point);

private:
    const MapStats &_mapStats;

    const std::vector<std::vector<std::vector<int>>> &_map;

    std::vector<std::vector<std::vector<int>>> _weightedGraphs;

    //std::vector<Node> _cachedMoveRadius;

    SDL_Point _cachedEnd;

    u32 _offset = 0;

    std::vector<std::vector<int>> convertToWeightedGraph(MovementType movementType);

    [[nodiscard]] const std::vector<std::vector<int>> &getWeightedGraph(MovementType movementType) const;

    std::vector<Node> getNeighbors(Node *node, const std::vector<std::vector<int>> &weightedGraph, SDL_Point start);

    static Node *pointInVector(const SDL_Point &point, std::vector<Node> &vector);

    [[maybe_unused]] static std::vector<SDL_Point> nodeToPointVector(std::vector<Node> &vector);

};


#endif //SDL_BASEGAME_PATHS_H
