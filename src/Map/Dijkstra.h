#ifndef SDL_BASEGAME_DIJKSTRA_H
#define SDL_BASEGAME_DIJKSTRA_H


#include <vector>
#include <SDL_rect.h>
#include <unordered_set>
#include "Helper/map_stat_helper.h"
#include <queue>

class PathFinder {
public:
    class Node {
    public:
        Node() {

        }

        SDL_Point _point;
        int _cost;
        SDL_Point _parent;
        int _totalCost;

        Node(SDL_Point point, int cost, SDL_Point parent, int totalCost) : _point(point), _cost(cost), _parent(parent),
                                                                           _totalCost(totalCost) {}

        Node(Node const &node) = default;

        bool operator>(const Node &other) const { return _totalCost > other._totalCost; }

    };

    struct MoveRadiusCacheKey {
        SDL_Point start;
        MovementType movementType;
        int moveRange;

        bool operator==(const MoveRadiusCacheKey &other) const {
            return start.x == other.start.x && start.y == other.start.y &&
                   movementType == other.movementType && moveRange == other.moveRange;
        }
    };

    struct MoveRadiusCacheKeyHash {
        std::size_t operator()(const MoveRadiusCacheKey &key) const {
            return std::hash<int>()(key.start.x)
                   ^ (std::hash<int>()(key.start.y) << 1)
                   ^ (std::hash<int>()(static_cast<int>(key.movementType)) << 2)
                   ^ (std::hash<int>()(key.moveRange) << 3);
        }
    };

    struct PointHash {
        std::size_t operator()(const SDL_Point &point) const {
            return std::hash<int>()(point.x) ^
                   (std::hash<int>()(point.y) << 1);
        }
    };

    struct PointEqual {
        bool operator()(const SDL_Point &a, const SDL_Point &b) const {
            return a.x == b.x && a.y == b.y;
        }
    };


    std::unordered_set<SDL_Point, PointHash, PointEqual>
    calculateMoveRadius(SDL_Point start, int movePoints, MovementType movementType);

    std::vector<SDL_Point> findShortestPath(SDL_Point start, SDL_Point end, int movePoints, MovementType movementType);

    PathFinder(const std::vector<std::vector<std::vector<int>>> &map, const MapStats &mapStats);

private:

    const MapStats &_mapStats;

    const std::vector<std::vector<std::vector<int>>> &_map;

    std::unordered_map<MovementType, std::vector<std::vector<int>>> _movementCostsTypeMapping; // initialized in constructor

    std::unordered_map<MoveRadiusCacheKey, std::unordered_map<SDL_Point, Node, PointHash, PointEqual>, MoveRadiusCacheKeyHash> _nodeCache;

    static std::unordered_set<SDL_Point, PathFinder::PointHash, PathFinder::PointEqual>
    extractPoints(const std::unordered_map<SDL_Point, Node, PointHash, PointEqual> &nodes);

    [[nodiscard]] const std::vector<std::vector<int>> *getWeightedGraph(MovementType movementType) const;

    std::vector<std::vector<int>> convertToWeightedGraph(MovementType movementType);

    std::vector<Node> getNeighbors(Node *node, std::vector<std::vector<int>> &weightedGraph);

    bool validPoint(SDL_Point point);

};


#endif //SDL_BASEGAME_DIJKSTRA_H
