#include "Dijkstra.h"
#include <iostream>

void printGraph(const std::vector<std::vector<int>> &graph) {
    for (const auto &row: graph) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

PathFinder::PathFinder(const std::vector<std::vector<std::vector<int>>> &map, const MapStats &mapStats)
        : _mapStats(mapStats), _map(map) {
    _movementCostsTypeMapping[MovementType::INFANTRY] = convertToWeightedGraph(MovementType::INFANTRY);
    _movementCostsTypeMapping[MovementType::MECH] = convertToWeightedGraph(MovementType::MECH);
    _movementCostsTypeMapping[MovementType::TIRE_A] = convertToWeightedGraph(MovementType::TIRE_A);
    _movementCostsTypeMapping[MovementType::TIRE_B] = convertToWeightedGraph(MovementType::TIRE_B);
    _movementCostsTypeMapping[MovementType::TREAD] = convertToWeightedGraph(MovementType::TREAD);
    _movementCostsTypeMapping[MovementType::AIR] = convertToWeightedGraph(MovementType::AIR);
    _movementCostsTypeMapping[MovementType::SEA] = convertToWeightedGraph(MovementType::SEA);
    _movementCostsTypeMapping[MovementType::TRANSPORT_BOAT] = convertToWeightedGraph(MovementType::TRANSPORT_BOAT);
}

std::vector<std::vector<int>> PathFinder::convertToWeightedGraph(MovementType movementType) {
    std::vector<std::vector<int>> weightedGraph;
    for (int y = 0; y < int(_map[0].size()); y++) {
        std::vector<int> row;
        row.reserve(int(_map[0][y].size()));
        for (int x = 0; x < int(_map[0][y].size()); x++) {
            row.push_back(_mapStats.getMovementCost(x, y, movementType));
        }
        weightedGraph.push_back(row);
    }

    return weightedGraph;
}

const std::vector<std::vector<int>> *PathFinder::getWeightedGraph(MovementType movementType) const {
    return &_movementCostsTypeMapping.at(movementType);
}

bool PathFinder::validPoint(SDL_Point point) {
    return point.x >= 0 && point.x < int(_map[0][0].size()) &&
           point.y >= 0 && point.y < int(_map[0].size());
}

std::vector<PathFinder::Node> PathFinder::getNeighbors(PathFinder::Node *node, std::vector<std::vector<int>> &weightedGraph) {
    if (node == nullptr) {
        throw std::invalid_argument("Node pointer cannot be null.");
    }
    std::vector<Node> neighbors;
    neighbors.reserve(4);
    SDL_Point parent = node->_point;
    SDL_Point newPoint;
    int cost;

    if (validPoint({parent.x - 1, parent.y})) {
        newPoint = {parent.x - 1, parent.y};
        cost = weightedGraph[newPoint.y][newPoint.x];
        Node n(newPoint, cost, parent, node->_totalCost + cost);
        neighbors.emplace_back(n);
    }
    if (validPoint({parent.x + 1, parent.y})) {
        newPoint = {parent.x + 1, parent.y};
        cost = weightedGraph[newPoint.y][newPoint.x];
        Node n(newPoint, cost, parent, node->_totalCost + cost);
        neighbors.emplace_back(n);
    }
    if (validPoint({parent.x, parent.y - 1})) {
        newPoint = {parent.x, parent.y - 1};
        cost = weightedGraph[newPoint.y][newPoint.x];
        Node n(newPoint, cost, parent, node->_totalCost + cost);
        neighbors.emplace_back(n);
    }
    if (validPoint({parent.x, parent.y + 1})) {
        newPoint = {parent.x, parent.y + 1};
        cost = weightedGraph[newPoint.y][newPoint.x];
        Node n(newPoint, cost, parent, node->_totalCost + cost);
        neighbors.emplace_back(n);
    }
    return neighbors;
}

std::unordered_set<SDL_Point, PathFinder::PointHash, PathFinder::PointEqual>
PathFinder::extractPoints(const std::unordered_map<SDL_Point, Node, PointHash,PointEqual> &nodes) {
    std::unordered_set<SDL_Point, PointHash, PointEqual> points;
    for (const auto &node: nodes) {
        SDL_Point p = node.second._point;
        points.insert(p);
    }
    return points;
}

std::unordered_set<SDL_Point, PathFinder::PointHash, PathFinder::PointEqual>
PathFinder::calculateMoveRadius(SDL_Point start, int movePoints, MovementType movementType) {
    MoveRadiusCacheKey key{start, movementType, movePoints};

    auto cached = _nodeCache.find(key);
    if (cached != _nodeCache.end()) {
        return extractPoints(cached->second);
    }

    std::unordered_map<SDL_Point, Node, PointHash, PointEqual> nodes = {
            {{start},Node(start, 0, start, 0)}
    };
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
    std::vector<std::vector<int>> weightedGraph = convertToWeightedGraph(movementType);

    queue.emplace(start, 0, start, 0);

    while (!queue.empty()) {
        Node currentNode = queue.top();
        queue.pop();

        for (const auto &neighbor: getNeighbors(&currentNode, weightedGraph)) {
            if (neighbor._totalCost <= movePoints && nodes.find(neighbor._point) == nodes.end()) {
                queue.push(neighbor);
                nodes[neighbor._point] = neighbor;
            }
        }
    }

    _nodeCache[key]=nodes;

    return extractPoints(nodes);
}
/*
std::vector<SDL_Point> PathFinder::findShortestPath(SDL_Point start, SDL_Point end, MovementType movementType) {
    return std::vector<SDL_Point>();
}*/

