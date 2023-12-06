#include "Paths.h"

void printGraph(const std::vector<std::vector<int>> &graph) {
    for (const auto &row: graph) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

Paths::Paths(const std::vector<std::vector<std::vector<int>>> &map, const MapStats &mapStats)
        : _mapStats(mapStats), _map(map) {
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::INFANTRY));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::MECH));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TIRE_A));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TIRE_B));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TREAD));
    //printGraph(_weightedGraphs[2]);
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::AIR));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::SEA));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TRANSPORT_BOAT));
}

std::vector<std::vector<int>> Paths::convertToWeightedGraph(MovementType movementType) {
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

const std::vector<std::vector<int>> &Paths::getWeightedGraph(MovementType movementType) const {
    return _weightedGraphs[static_cast<int>(movementType)];
}

std::vector<Paths::Node> Paths::getNeighbors(Node *node, const std::vector<std::vector<int>> &weightedGraph) {
    if (node == nullptr) {
        throw std::invalid_argument("Node pointer cannot be null.");
    }
    std::vector<Node> neighbors;
    SDL_Point parent = node->_coordinates;
    SDL_Point neighbor;
    int cost;

    // left
    if (validPoint({parent.x - 1, parent.y})) {
        neighbor = {parent.x - 1, parent.y};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max()) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    // right
    if (validPoint({parent.x + 1, parent.y})) {
        neighbor = {parent.x + 1, parent.y};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max()) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    // up
    if (validPoint({parent.x, parent.y - 1})) {
        neighbor = {parent.x, parent.y - 1};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max()) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    // down
    if (validPoint({parent.x, parent.y + 1})) {
        neighbor = {parent.x, parent.y + 1};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max()) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    return neighbors;
}

bool Paths::validPoint(const SDL_Point &point) {
    return point.x >= 0 && point.x < int(_map[0][0].size()) &&
           point.y >= 0 && point.y < int(_map[0].size());
}

Paths::Node *Paths::pointInVector(const SDL_Point &point, std::vector<Node> &vector) {
    for (auto &i: vector) {
        if (i._coordinates.x == point.x && i._coordinates.y == point.y) {
            return &i;
        }
    }
    return nullptr;
}

std::vector<SDL_Point> Paths::nodeToPointVector(std::vector<Node> &vector) {
    std::vector<SDL_Point> points;
    points.reserve(vector.size());
    for (auto &i: vector) {
        points.push_back(i._coordinates);
    }
    return points;
}

bool Paths::mouseInRadius(SDL_Point pos) {
    if (_cachedMoveRadius.empty()) {
        return false;
    }

    int x = pos.x / 32;
    int y = pos.y / 32;

    if (x < 0 || x >= int(_map[0][0].size()) || y < 0 || y >= int(_map[0].size())) {
        return false;
    }

    for (auto &i: _cachedMoveRadius) {
        if (i._coordinates.x == x && i._coordinates.y == y) {
            return true;
        }
    }
    return false;
}

void Paths::drawMoveRadius(u32 frame) {
    SDL_Renderer *renderer = RS::getInstance().get();
    SDL_Texture *texture = RS::getInstance().getTexture();

    SDL_Rect srcRect;
    SDL_Rect destRect;
    // 198


    _offset = frame / 3 % 16;


    srcRect.w = srcRect.h = 32;
    srcRect.x = int(_offset) * 32;
    srcRect.y = 176;

    destRect.w = destRect.h = 16 * 2 - 2;

    for (auto &i: _cachedMoveRadius) {
        destRect.x = i._coordinates.x * 16 * 2 + 1;
        destRect.y = i._coordinates.y * 16 * 2 + 1;
        //_offset = (_offset + 1) % 3;
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &destRect);
    }
}


std::vector<SDL_Point> Paths::getMoveRadius(SDL_Point start, MovementType movementType, int actionPoints) {
    if (!_cachedMoveRadius.empty() && _cachedMoveRadius[0]._coordinates.x == start.x &&
        _cachedMoveRadius[0]._coordinates.y == start.y) {
        return nodeToPointVector(_cachedMoveRadius);
    }
    std::vector<Node> moveRadius;
    std::vector<std::vector<int>> weightedGraph = getWeightedGraph(movementType);
    std::priority_queue<Node, std::vector<Node>, std::greater<>> queue;

    Node startNode(start, start, 0, 0);
    queue.emplace(startNode);
    moveRadius.push_back(startNode);

    while (!queue.empty()) {
        Node node = queue.top();
        queue.pop();


        std::vector<Node> neighbors = getNeighbors(&node, weightedGraph);
        for (Node neighbor: neighbors) {
            if (neighbor._totalCost <= actionPoints && pointInVector(neighbor._coordinates, moveRadius) == nullptr) {
                moveRadius.push_back(neighbor);
                queue.emplace(neighbor);
            }
        }
    }

    _cachedMoveRadius = moveRadius;

    return nodeToPointVector(moveRadius);
}

std::vector<SDL_Point> Paths::getPath(SDL_Point start, SDL_Point end, MovementType movementType, int actionPoints) {
    std::vector<SDL_Point> path;
    std::vector<Node> moveRadius = _cachedMoveRadius;
    Node *currentNode = pointInVector(end, moveRadius);

    if (currentNode == nullptr) {
        throw std::invalid_argument("End point is not in the move radius");
    }


    while (!(currentNode->_coordinates.x == start.x && currentNode->_coordinates.y == start.y)) {
        path.push_back(currentNode->_coordinates);
        currentNode = pointInVector(currentNode->_parentCoordinates, moveRadius);
        if (currentNode == nullptr) {
            throw std::invalid_argument("Invalid path: broken parent linkage");
        }
    }

    path.push_back(start);
    //std::reverse(path.begin(), path.end());

    return path;
}

