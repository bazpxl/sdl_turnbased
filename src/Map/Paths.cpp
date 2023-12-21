#include "Paths.h"

[[maybe_unused]] void printGraph(const std::vector<std::vector<int>> &graph) {
    for (const auto &row: graph) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

Paths::Paths(const std::vector<std::vector<std::vector<int>>> &map, const MapStats &mapStats) // NOLINT(*-pro-type-member-init)
        : _mapStats(mapStats), _map(map) {
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::INFANTRY));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::MECH));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TIRE_A));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TIRE_B));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TREAD));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::AIR));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::SEA));
    _weightedGraphs.emplace_back(convertToWeightedGraph(MovementType::TRANSPORT_BOAT));

    _arrowPos = std::vector<int>(30);
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

std::vector<std::vector<int>> Paths::convertToWeightedGraph(const MovementType movementType) const {
    std::vector<std::vector<int>> weightedGraph;
    for (int y = 0; y < static_cast<int>(_map[0].size()); y++) {
        std::vector<int> row;
        row.reserve(static_cast<int>(_map[0][y].size()));
        for (int x = 0; x < static_cast<int>(_map[0][y].size()); x++) {
            row.push_back(_mapStats.getMovementCost({x, y}, movementType));
        }
        weightedGraph.push_back(row);
    }

    return weightedGraph;
}

const std::vector<std::vector<int>> &Paths::getWeightedGraph(MovementType movementType) const {
    return _weightedGraphs[static_cast<int>(movementType)];
}

std::vector<Paths::Node> Paths::getNeighbors(const Node *node, const std::vector<std::vector<int>> &weightedGraph, const SDL_Point start) {

    std::vector<Node> neighbors;
    const SDL_Point parent = node->_coordinates;
    SDL_Point neighbor;
    int cost;

    // left
    if (validPoint({parent.x - 1, parent.y})) {
        neighbor = {parent.x - 1, parent.y};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max() && _mapStats.inUnitMapAndSameTeam(neighbor, start) == -1) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    // right
    if (validPoint({parent.x + 1, parent.y})) {
        neighbor = {parent.x + 1, parent.y};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max() && _mapStats.inUnitMapAndSameTeam(neighbor, start) == -1) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    // up
    if (validPoint({parent.x, parent.y - 1})) {
        neighbor = {parent.x, parent.y - 1};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max() && _mapStats.inUnitMapAndSameTeam(neighbor, start) == -1) {
            Node neighborNode(neighbor, parent, cost, node->_totalCost + cost);
            neighbors.emplace_back(neighborNode);
        }
    }

    // down
    if (validPoint({parent.x, parent.y + 1})) {
        neighbor = {parent.x, parent.y + 1};
        cost = weightedGraph[neighbor.y][neighbor.x];
        if (cost != std::numeric_limits<int>::max() && _mapStats.inUnitMapAndSameTeam(neighbor, start) == -1) {
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

[[maybe_unused]] std::vector<SDL_Point> Paths::nodeToPointVector(std::vector<Node> &vector) {
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

    for (const auto &i: radius) {
        if (i._coordinates.x == x && i._coordinates.y == y) {
            return true;
        }
    }
    return false;
}

void Paths::drawMoveRadius(u32 frame, std::vector<Node> &radius,std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> &attackRadius) {
    if (!radius.empty()) {
        SDL_Renderer *renderer = RS::getInstance().get();
        SDL_Texture *texture = RS::getInstance().getTexture();
        auto _attackRadius = attackRadius;

        SDL_Rect srcRect;
        SDL_Rect destRect;
        // 198


        _offset = frame / 3 % 16;


        srcRect.w = srcRect.h = 32;
        srcRect.x = int(_offset) * 32;
        srcRect.y = 176;

        destRect.w = destRect.h = TILE_SIZE - 2;

        for (auto &i: radius) {
            if (_attackRadius.find(i._coordinates) != _attackRadius.end()) {
                _attackRadius.erase(i._coordinates);
            }
            destRect.x = i._coordinates.x * TILE_SIZE + 1;
            destRect.y = i._coordinates.y * TILE_SIZE + 1;
            //_offset = (_offset + 1) % 3;
            SDL_SetTextureColorMod(texture, 255,205,101);
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
            SDL_SetTextureColorMod(texture, 255, 255, 255);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &destRect);
        }
        for(auto &i: _attackRadius){
            destRect.x = i.first.x * TILE_SIZE + 1;
            destRect.y = i.first.y * TILE_SIZE + 1;
            if(i.second == 0){
                SDL_SetTextureColorMod(texture, 205,74,49);
                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
                SDL_SetTextureColorMod(texture, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &destRect);
            }else{
                SDL_SetTextureColorMod(texture, 220,194,166);
                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
                SDL_SetTextureColorMod(texture, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &destRect);
            }
        }
    }
}

void drawTile(int tileIndex, SDL_Rect &destRect, int imgSizeX, int tileSize) {
    SDL_Renderer *renderer = RS::getInstance().get();
    SDL_Texture *texture = RS::getInstance().getTexture();
    int tilesPerRow = imgSizeX / tileSize;

    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = TILE_SIZE;

    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Paths::drawPath(std::vector<SDL_Point> &path) {
    if (path.size() < 2) return;


    std::vector<int> renderQueue;

    auto addToRenderQueue = [&](int tileIndex) {
        renderQueue.push_back(tileIndex);
    };

    // Erste Position (Pfeil)
    if (path[1].x > path[0].x) addToRenderQueue(_arrowPos[static_cast<int>(AT::LEFT_ARROW)]);
    else if (path[1].x < path[0].x) addToRenderQueue(_arrowPos[static_cast<int>(AT::RIGHT_ARROW)]);
    else if (path[1].y > path[0].y) addToRenderQueue(_arrowPos[static_cast<int>(AT::UP_ARROW)]);
    else if (path[1].y < path[0].y) addToRenderQueue(_arrowPos[static_cast<int>(AT::DOWN_ARROW)]);

    // Zwischenpositionen (Verbindungsstücke)
    for (size_t i = 1; i < path.size() - 1; i++) {
        SDL_Point prev = {path[i - 1].x - path[i].x, path[i - 1].y - path[i].y};
        SDL_Point next = {path[i + 1].x - path[i].x, path[i + 1].y - path[i].y};
        // 1 = rechts,unten, -1 = links,oben
        if (next.x == 1) { // Rechts vom aktuellen Punkt (Nachfolger)
            if (prev.x == -1) { // Links (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_CONNECT)]);
            } else if (prev.y == -1) { // Oben (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_RIGHT_CONNECT)]);
            } else if (prev.y == 1) { // Unten (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_RIGHT_CONNECT)]);
            }
        } else if (next.x == -1) { // Links vom aktuellen Punkt (Nachfolger)
            if (prev.x == 1) { // Rechts (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::HOR_CONNECT)]);
            } else if (prev.y == -1) { // Oben (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_LEFT_CONNECT)]);
            } else if (prev.y == 1) { // Unten (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_LEFT_CONNECT)]);
            }
        } else if (next.y == -1) { // Oben vom aktuellen Punkt (Nachfolger)
            if (prev.y == 1) { // Unten (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::VER_CONNECT)]);
            } else if (prev.x == -1) { // Links (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_LEFT_CONNECT)]);
            } else if (prev.x == 1) { // Rechts (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::DOWN_RIGHT_CONNECT)]);
            }
        } else if (next.y == 1) { // Unten vom aktuellen Punkt (Nachfolger)
            if (prev.y == -1) { // Oben (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::VER_CONNECT)]);
            } else if (prev.x == -1) { // Links (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_LEFT_CONNECT)]);
            } else if (prev.x == 1) { // Rechts (Vorgänger)
                renderQueue.push_back(_arrowPos[static_cast<int>(AT::UP_RIGHT_CONNECT)]);
            }
        }
    }
    // Letzte Position (Ende)
    size_t last = path.size() - 1;
    if (path[last - 1].x > path[last].x) addToRenderQueue(_arrowPos[static_cast<int>(AT::HOR_END_LEFT)]);
    else if (path[last - 1].x < path[last].x) addToRenderQueue(_arrowPos[static_cast<int>(AT::HOR_END_RIGHT)]);
    else if (path[last - 1].y > path[last].y) addToRenderQueue(_arrowPos[static_cast<int>(AT::VER_END_UP)]);
    else if (path[last - 1].y < path[last].y) addToRenderQueue(_arrowPos[static_cast<int>(AT::VER_END_DOWN)]);

    for (size_t i = 0; i < renderQueue.size(); i++) {
        const int tileIndex = renderQueue[i];
        SDL_Rect destRect;
        destRect.x = path[i].x * TILE_SIZE;
        destRect.y = path[i].y * TILE_SIZE;
        drawTile(tileIndex, destRect, 512, 16);
    }
}


std::vector<Paths::Node>
Paths::getMoveRadius(const SDL_Point start, const MovementType movementType, const int actionPoints, std::vector<Node> &radius) {
    if (!radius.empty() && radius[0]._coordinates.x == start.x &&
        radius[0]._coordinates.y == start.y) {
        return radius;
    }
    std::vector<Node> moveRadius;
    const std::vector<std::vector<int>> weightedGraph = getWeightedGraph(movementType);
    std::priority_queue<Node, std::vector<Node>, std::greater<>> queue;

    Node startNode(start, start, 0, 0);
    queue.emplace(startNode);
    moveRadius.push_back(startNode);

    while (!queue.empty()) {
        Node node = queue.top();
        queue.pop();


        for (std::vector<Node> neighbors = getNeighbors(&node, weightedGraph, start); Node neighbor: neighbors) {
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


std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> Paths::getAttackRadius(SDL_Point start, int range, std::vector<Node> &radius) {
    std::unordered_set<SDL_Point, Paths::SDLPointHash, Paths::SDLPointEqual> attackRadiusSet;
    std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> attackRadius;

    for (auto &node: radius) {
        if (std::abs(node._coordinates.x - start.x) + std::abs(node._coordinates.y - start.y) <= range) {
            std::vector<SDL_Point> neighbors = {
                    {node._coordinates.x - 1, node._coordinates.y},
                    {node._coordinates.x + 1, node._coordinates.y},
                    {node._coordinates.x,     node._coordinates.y - 1},
                    {node._coordinates.x,     node._coordinates.y + 1}
            };

            for (auto &neighbor: neighbors) {
                if (validPoint(neighbor) && attackRadiusSet.find(neighbor) == attackRadiusSet.end()) {
                    attackRadiusSet.insert(neighbor);
                    _mapStats.inUnitMapAndSameTeam(neighbor, start) == 1 ? attackRadius[neighbor] = 1 : _mapStats.inUnitMapAndSameTeam(neighbor, start) == 0 ? attackRadius[neighbor] = 0 : attackRadius[neighbor] = -1;
                }
            }
        }
    }

    return attackRadius;
}


