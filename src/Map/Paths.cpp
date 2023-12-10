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
    std::cout << "Creating weighted graphs..." << std::endl;
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::INFANTRY));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::MECH));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TIRE_A));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TIRE_B));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TREAD));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::AIR));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::SEA));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TRANSPORT_BOAT));

    _arrowPos = std::vector<int>(21);
    _arrowPos[static_cast<int>(AT::LEFT_ARROW)] = 69;
    _arrowPos[static_cast<int>(AT::RIGHT_ARROW)] = 71;
    _arrowPos[static_cast<int>(AT::UP_ARROW)] = 68;
    _arrowPos[static_cast<int>(AT::DOWN_ARROW)] = 132;
    _arrowPos[static_cast<int>(AT::HOR_END_LEFT)] = 306;
    _arrowPos[static_cast<int>(AT::HOR_END_RIGHT)] = 338;
    _arrowPos[static_cast<int>(AT::VER_END_UP)] = 307;
    _arrowPos[static_cast<int>(AT::VER_END_DOWN)] = 339;
    _arrowPos[static_cast<int>(AT::HOR_CONNECT)] = 70;
    _arrowPos[static_cast<int>(AT::VER_CONNECT)] = 100;
    _arrowPos[static_cast<int>(AT::UP_RIGHT_CONNECT)] = 101;
    _arrowPos[static_cast<int>(AT::UP_LEFT_CONNECT)] = 102;
    _arrowPos[static_cast<int>(AT::DOWN_RIGHT_CONNECT)] = 133;
    _arrowPos[static_cast<int>(AT::DOWN_LEFT_CONNECT)] = 134;
    _arrowPos[static_cast<int>(AT::VERT_UP_RIGHT_END)] = 242;
    _arrowPos[static_cast<int>(AT::VERT_UP_LEFT_END)] = 243;
    _arrowPos[static_cast<int>(AT::VERT_DOWN_RIGHT_END)] = 274;
    _arrowPos[static_cast<int>(AT::VERT_DOWN_LEFT_END)] = 275;
    _arrowPos[static_cast<int>(AT::HOR_UP_RIGHT_END)] = 210;
    _arrowPos[static_cast<int>(AT::HOR_UP_LEFT_END)] = 211;
    _arrowPos[static_cast<int>(AT::HOR_DOWN_RIGHT_END)] = 179;
    _arrowPos[static_cast<int>(AT::HOR_DOWN_LEFT_END)] = 178;

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

bool Paths::mouseInRadius(SDL_Point pos, std::vector<Node> &radius) {
    if (radius.empty()) {
        return false;
    }

    int x = pos.x;
    int y = pos.y;

    if (x < 0 || x >= int(_map[0][0].size()) || y < 0 || y >= int(_map[0].size())) {
        return false;
    }

    for (auto &i: radius) {
        if (i._coordinates.x == x && i._coordinates.y == y) {
            return true;
        }
    }
    return false;
}

void Paths::drawMoveRadius(u32 frame, std::vector<Node> &radius) {
    if (!radius.empty()) {
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

        for (auto &i: radius) {
            destRect.x = i._coordinates.x * 16 * 2 + 1;
            destRect.y = i._coordinates.y * 16 * 2 + 1;
            //_offset = (_offset + 1) % 3;
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &destRect);
        }
    }
}

void Paths::drawPath(std::vector<SDL_Point> &path) {
    SDL_Renderer *renderer = RS::getInstance().get();
    SDL_Texture *texture = RS::getInstance().getTexture();
    std::vector<int> renderQueue(path.size());
    if (!path.empty()) {
        for (size_t i = 0; i < path.size() - 1; i++) {
            if (i == 0) {
                if (path[i + 1].x > path[i].x && path[i + 1].y == path[i].y) {
                    renderQueue.push_back(_arrowPos[static_cast<int>(AT::RIGHT_ARROW)]);
                } else if (path[i + 1].x < path[i].x && path[i + 1].y == path[i].y) {
                    renderQueue.push_back(_arrowPos[static_cast<int>(AT::LEFT_ARROW)]);
                } else if (path[i + 1].x == path[i].x && path[i + 1].y > path[i].y) {
                    renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_ARROW)]);
                } else if (path[i + 1].x == path[i].x && path[i + 1].y < path[i].y) {
                    renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_ARROW)]);
                }
            } else if (i == path.size() - 1) {
                if (path[i - 1].x > path[i].x && path[i - 1].y == path[i].y) { // Rechts (Vorgänger)
                    if (path[i].y == path[i + 1].y) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_END_LEFT)]);
                    } else if (path[i].y < path[i + 1].y) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VERT_UP_LEFT_END)]);
                    } else if (path[i].y > path[i + 1].y) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VERT_DOWN_LEFT_END)]);
                    }
                } else if (path[i - 1].x < path[i].x && path[i - 1].y == path[i].y) { // Links (Vorgänger)
                    if (path[i].y == path[i + 1].y) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_END_RIGHT)]);
                    } else if (path[i].y < path[i + 1].y) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VERT_UP_RIGHT_END)]);
                    } else if (path[i].y > path[i + 1].y) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VERT_DOWN_RIGHT_END)]);
                    }
                } else if (path[i - 1].x == path[i].x && path[i - 1].y > path[i].y) { // Oben (Vorgänger)
                    if (path[i].x == path[i + 1].x) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VER_END_DOWN)]);
                    } else if (path[i].x < path[i + 1].x) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_DOWN_LEFT_END)]);
                    } else if (path[i].x > path[i + 1].x) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_DOWN_RIGHT_END)]);
                    }
                } else if (path[i - 1].x == path[i].x && path[i - 1].y < path[i].y) { // Unten (Vorgänger)
                    if (path[i].x == path[i + 1].x) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VER_END_UP)]);
                    } else if (path[i].x < path[i + 1].x) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_UP_LEFT_END)]);
                    } else if (path[i].x > path[i + 1].x) {
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_UP_RIGHT_END)]);
                    }
                }

            } else {
                if (path[i + 1].x > path[i].x &&
                    path[i + 1].y == path[i].y) { // Rechts vom aktuellen Punkt (Nachfolger)
                    if (path[i - 1].x < path[i].x && path[i - 1].y == path[i].y) { // Links (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_CONNECT)]);
                    } else if (path[i - 1].x == path[i].x && path[i - 1].y < path[i].y) { // Oben (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_RIGHT_CONNECT)]);
                    } else if (path[i - 1].x == path[i].x && path[i - 1].y > path[i].y) { // Unten (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_RIGHT_CONNECT)]);
                    }
                } else if (path[i + 1].x < path[i].x &&
                           path[i + 1].y == path[i].y) { // Links vom aktuellen Punkt (Nachfolger)
                    if (path[i - 1].x > path[i].x && path[i - 1].y == path[i].y) { // Rechts (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_CONNECT)]);
                    } else if (path[i - 1].x == path[i].x && path[i - 1].y < path[i].y) { // Oben (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_LEFT_CONNECT)]);
                    } else if (path[i - 1].x == path[i].x && path[i - 1].y > path[i].y) { // Unten (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_LEFT_CONNECT)]);
                    }
                } else if (path[i + 1].x == path[i].x &&
                           path[i + 1].y > path[i].y) { // Oben vom aktuellen Punkt (Nachfolger)
                    if (path[i - 1].x == path[i].x && path[i - 1].y < path[i].y) { // Unten (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VER_CONNECT)]);
                    } else if (path[i - 1].x < path[i].x && path[i - 1].y == path[i].y) { // Links (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_LEFT_CONNECT)]);
                    } else if (path[i - 1].x > path[i].x && path[i - 1].y == path[i].y) { // Rechts (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_RIGHT_CONNECT)]);
                    }
                } else if (path[i + 1].x == path[i].x &&
                           path[i + 1].y < path[i].y) { // Unten vom aktuellen Punkt (Nachfolger)
                    if (path[i - 1].x == path[i].x && path[i - 1].y > path[i].y) { // Oben (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::VER_CONNECT)]);
                    } else if (path[i - 1].x < path[i].x && path[i - 1].y == path[i].y) { // Links (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_LEFT_CONNECT)]);
                    } else if (path[i - 1].x > path[i].x && path[i - 1].y == path[i].y) { // Rechts (Vorgänger)
                        renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_RIGHT_CONNECT)]);
                    }
                }
            }
        }

        int tilesPerRow = 512/32;
        SDL_Rect srcRect;
        SDL_Rect destRect;
        for (size_t i = 0; i < path.size(); i++) {
            SDL_Point point = path[i];
            int tileIndex = renderQueue[i];

            srcRect.x = (tileIndex % tilesPerRow) * 32;
            srcRect.y = (tileIndex / tilesPerRow) * 32;

            destRect.x = point.x * 32;
            destRect.y = point.y * 32;
            destRect.w = destRect.h = 32; // Größe des Quadrats

            // Zeichne die Textur
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        }
    }
}


std::vector<Paths::Node>
Paths::getMoveRadius(SDL_Point start, MovementType movementType, int actionPoints, std::vector<Node> &radius) {
    if (!radius.empty() && radius[0]._coordinates.x == start.x &&
        radius[0]._coordinates.y == start.y) {
        return radius;
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
            if (neighbor._totalCost <= actionPoints &&
                pointInVector(neighbor._coordinates, moveRadius) == nullptr) {
                moveRadius.push_back(neighbor);
                queue.emplace(neighbor);
            }
        }
    }

    //_cachedMoveRadius = moveRadius;

    return moveRadius;
}

std::vector<SDL_Point> Paths::getPath(SDL_Point start, SDL_Point end, std::vector<Node> &radius) {
    std::vector<SDL_Point> path;
    Node *currentNode = pointInVector(end, radius);
    std::cout << "Start" << std::endl;
    if (currentNode == nullptr) {
        currentNode = pointInVector(_cachedEnd, radius);
    }

    while (!(currentNode->_coordinates.x == start.x && currentNode->_coordinates.y == start.y)) {
        path.push_back(currentNode->_coordinates);
        currentNode = pointInVector(currentNode->_parentCoordinates, radius);
        if (currentNode == nullptr) {
            throw std::invalid_argument("Invalid path: broken parent linkage");
        }
    }

    path.push_back(start);
    //std::reverse(path.begin(), path.end());
    _cachedEnd = path[0];
    return path;
}

