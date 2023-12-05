#include "Dijkstra.h"
#include <iostream>

/*void printGraph(const std::vector<std::vector<int>> &graph) {
    for (const auto &row: graph) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}*/

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

// get a weighted graph from the map
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

// retruns a pointer to the weighted graph for the given movement type
const std::vector<std::vector<int>> &PathFinder::getWeightedGraph(MovementType movementType) const {
    return _movementCostsTypeMapping.at(movementType);
}

// helper to check if a point is within the map
bool PathFinder::validPoint(SDL_Point point) {
    return point.x >= 0 && point.x < int(_map[0][0].size()) &&
           point.y >= 0 && point.y < int(_map[0].size());
}

// get the neighbors of a node in a 4-connected grid
std::vector<PathFinder::Node>
PathFinder::getNeighbors(PathFinder::Node *node, std::vector<std::vector<int>> &weightedGraph) {
    if (node == nullptr) {
        throw std::invalid_argument("Node pointer cannot be null.");
    }
    std::vector<Node> neighbors;
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

// helper to extract the points from a map of nodes
std::unordered_set<SDL_Point, PathFinder::PointHash, PathFinder::PointEqual>
PathFinder::extractPoints(const std::unordered_map<SDL_Point, Node, PointHash, PointEqual> &nodes) {
    std::unordered_set<SDL_Point, PointHash, PointEqual> points;
    for (const auto &node: nodes) {
        SDL_Point p = node.second._point;
        points.insert(p);
    }
    return points;
}

// calculate the move radius for a given start point, movement type and move points
std::unordered_set<SDL_Point, PathFinder::PointHash, PathFinder::PointEqual>
PathFinder::calculateMoveRadius(SDL_Point start, int movePoints, MovementType movementType) {
    MoveRadiusCacheKey key{start, movementType, movePoints};

    // check if we already calculated the move radius for this key
    auto cached = _nodeCache.find(key);
    if (cached != _nodeCache.end()) {
        return extractPoints(cached->second);
    }

    std::unordered_map<SDL_Point, Node, PointHash, PointEqual> nodes = {
            {start, Node(start, 0, start, 0)}
    };
    std::priority_queue<Node, std::vector<Node>, std::greater<>> queue;
    std::vector<std::vector<int>> weightedGraph = getWeightedGraph(movementType);

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

    _nodeCache[key] = nodes;

    // returns the points of the nodes
    return extractPoints(nodes);
}

std::vector<SDL_Point> PathFinder::findShortestPath(SDL_Point start, SDL_Point end, int movePoints, MovementType movementType) {
    MoveRadiusCacheKey key{start, movementType, movePoints};
    if(_nodeCache.find(key) == _nodeCache.end()){
        calculateMoveRadius(start,movePoints,movementType);
    }
    const auto &cache = _nodeCache.find(key);
    const auto &nodes = cache->second;

    std::vector<SDL_Point> path;

    SDL_Point current = end;

    while(current.x != start.x || current.y != start.y){
        auto nodeIt = nodes.find(current);
        if (nodeIt == nodes.end()) {
            throw std::invalid_argument("Invalid path");
        }
        path.push_back(current);
        current = nodeIt->second._parent;
    }

    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return path;
}


