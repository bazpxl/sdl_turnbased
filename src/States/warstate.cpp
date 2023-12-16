#include "examplegame.h"

TTF_Font *indexFont;

std::unique_ptr<Unit> infantryUnit;
std::unique_ptr<Unit> mechUnit;
std::unique_ptr<Unit> infantryUnit2;


bool operator==(const SDL_Point &lhs, const SDL_Point &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const SDL_Point &lhs, const SDL_Point &rhs) {
    return !(lhs == rhs);
}

void WarState::Init() {

    RS::getInstance().init(renderer);

    map.push_back(csvToMap(BasePath "asset/map/pvp/bg.csv"));
    map.push_back(csvToMap(BasePath"asset/map/pvp/map.csv"));

    initUnitMap();

    paths = new Paths(map, MapStats::getInstance(&map, &unitMap));

    cc = new CombatCalculator(MapStats::getInstance(&map, &unitMap));

    loadTileset("asset/graphic/NewTiles.png");

    infantryUnit = UnitFactory::createUnit(UnitType::INFANTRY, 2, 9, 2);
    mechUnit = UnitFactory::createUnit(UnitType::MECH, 1, 8, 2);
    infantryUnit2 = UnitFactory::createUnit(UnitType::INFANTRY, 13, 2, 1);
    std::cout <<infantryUnit2->getMoveRange() << std::endl;
    std::cout <<infantryUnit2->getAttackRange() << std::endl;


    unitMap[2][13] = infantryUnit2.get();
    unitMap[9][2] = infantryUnit.get();
    unitMap[8][1] = mechUnit.get();

    // Kann ggf weg
    indexFont = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 10);

    std::cout << cc->calculateDamage(*infantryUnit, *infantryUnit2) << std::endl;
}

void WarState::UnInit() {
    delete (paths);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(indexFont);
}

bool WarState::HandleEvent(const Event &event) {
    updateMouseIndex(event);

    if (isLeftMouseButtonDown(event)) {
        handleLeftMouseButtonDown();
    }

    if (isLeftMouseButtonUp(event)) {
        handleLeftMouseButtonUp();
    }

    processUnitSelectionAndMovement(event);
    return false;
}


void WarState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    // TODO: Implement Unit Movement Update in here
}

void WarState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMap();

    //draw radius and path
    paths->drawMoveRadius(frame, radius, attackRadius);

    paths->drawPath(path);

    //draw units
    drawUnits();

}

/* ##############################################
 * ##  FUNCTIONS CLEANUP                       ##
 * ##############################################
 * */

std::vector<std::vector<int>> WarState::csvToMap(const string &filename) {
    std::vector<std::vector<int>> map;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << std::endl;
        return map;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }
        map.push_back(row);
    }
    return map;
}

void WarState::renderTileset(TTF_Font *font, SDL_Point imgSize, int tileSize) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    int tilesPerRow = imgSize.x / tileSize;
    int tilesPerColumn = imgSize.y / tileSize;
    srcRect.w = srcRect.h = tileSize;
    destRect.w = destRect.h = tileSize * 2;

    for (int y = 0; y < tilesPerColumn; ++y) {
        for (int x = 0; x < tilesPerRow; ++x) {
            srcRect.x = x * tileSize;
            srcRect.y = y * tileSize;

            destRect.x = x * tileSize * 2;
            destRect.y = y * tileSize * 2;

            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

            int index = y * tilesPerRow + x;
            std::string indexText = std::to_string(index);
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, indexText.c_str(), {255, 255, 255, 255});
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRect = {destRect.x, destRect.y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}


void WarState::drawTile(int tileIndex, SDL_Rect &destRect, int imgSizeX, int tileSize) {
    int tilesPerRow = imgSizeX / tileSize;

    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = 32;

    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void WarState::drawMap() {
    SDL_Rect destRect;
    for (auto &layer: map) {
        for (size_t j = 0; j < layer.size(); j++) {
            for (size_t k = 0; k < layer[j].size(); k++) {
                destRect.x = int(k) * 32;
                destRect.y = int(j) * 32;
                drawTile(layer[j][k], destRect, 512);
            }
        }
    }
}

void WarState::drawUnits() {
    for (auto &row: unitMap) {
        for (auto &unit: row) {
            if (unit != nullptr) {
                unit->draw();
            }
        }
    }
}

void WarState::loadTileset(const std::string &filename) {
    std::string fullPath = BasePath + filename;
    texture = IMG_LoadTexture(renderer, fullPath.c_str());
    if (!texture) {
        std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
        return;
    }
    RS::getInstance().setTexture(texture);
    Unit::setTexture(texture);
}

void WarState::initUnitMap() {
    if (!map.empty() && !map[0].empty()) {
        size_t numRows = map[0].size();
        size_t numCols = map[0][0].size();

        unitMap.resize(numRows);
        for (auto &row: unitMap) {
            row.resize(numCols, nullptr);
        }
    }
    unitMap.reserve(map.capacity());
    Unit::setUnitMap(&unitMap);
}

void WarState::updateMouseIndex(const Event &event) {
    if (event.type == SDL_MOUSEMOTION) {
        mouseIndex.x = event.motion.x / 32;
        mouseIndex.y = event.motion.y / 32;
    }
}

bool WarState::isLeftMouseButtonDown(const Event &event) {
    return event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT;
}

bool WarState::isLeftMouseButtonUp(const Event &event) {
    return event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT;
}

void WarState::handleLeftMouseButtonDown() {
    mouseDown = true;
}

void WarState::handleLeftMouseButtonUp() {
    mouseDown = false;
    sameClick = false;
}

void WarState::clearSelectionAndPath() {
    path.clear();
    radius.clear();
    attackRadius.clear();
    selected = nullptr;
}

void WarState::processUnitSelectionAndMovement(const Event &event) {
    Unit *unit;
    if (mouseIndex.x < 0 || mouseIndex.y < 0 || mouseIndex.x >= int(unitMap[0].size()) || mouseIndex.y >= int(unitMap.size())) {
        unit = nullptr;
    } else {
        unit = unitMap[mouseIndex.y][mouseIndex.x];
    }
    if ((mouseDown && unit) || selected) {
        handleUnitInteraction(unit, event);
    } else {
        clearSelectionAndPath();
    }
}

void WarState::handleUnitInteraction(Unit *unit, const Event &event) {

    if (unit && unit->getTeam() == 1 && !unit->hasMoved()) {
        selected = unit;
    }
    if (unit && !selected && (unit->getTeam() != 1 || unit->hasMoved())) {

        path.clear();
        radius = paths->getMoveRadius(mouseIndex, unit->getMovementType(), unit->getMoveRange(), radius);
        attackRadius = paths->getAttackRadius(mouseIndex, unit->getAttackRange(), radius);

    } else if (selected) {
        //radius = paths->getMoveRadius(selected->getCoordinates(),selected->getMovementType(),selected->getMoveRange(),radius);
        radius = paths->getMoveRadius(selected->getCoordinates(),selected->getMovementType(),selected->getMoveRange(),radius);
        attackRadius = paths->getAttackRadius(selected->getCoordinates(), selected->getAttackRange(), radius);
        path = paths->getPath(selected->getCoordinates(), mouseIndex, radius);


        if (!sameClick && isLeftMouseButtonDown(event) && paths->mouseInRadius(mouseIndex, radius) &&
            mouseIndex != selected->getCoordinates()) {
            unitMap[selected->getCoordinates().y][selected->getCoordinates().x] = nullptr;
            unitMap[mouseIndex.y][mouseIndex.x] = selected;
            selected->setCoordinates(mouseIndex.x, mouseIndex.y);
            //selected->setHasMoved(true);
            sameClick = true;
            clearSelectionAndPath();
        } else if (isLeftMouseButtonDown(event) && !paths->mouseInRadius(mouseIndex, radius)) {
            clearSelectionAndPath();
        }

    }

}