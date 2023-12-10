#include <fstream>
#include <sstream>
#include <iostream>
#include "Units/Unit.h"
#include "Helper/map_stat_helper.h"
#include "examplegame.h"
#include "Map/Dijkstra.h"
#include "Map/Paths.h"

SDL_Texture *texture;
bool radiusVisible = false;
SDL_Point mousePos;
std::vector<SDL_Point> uPath;
bool mouseDown = false;
bool sameClick = true;


TTF_Font *indexFont;


std::unique_ptr<Unit> infantryUnit;
std::unique_ptr<Unit> mechUnit;
std::unique_ptr<Unit> infantryUnit2;

#include <SDL_rect.h>

bool operator==(const SDL_Point &lhs, const SDL_Point &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const SDL_Point &lhs, const SDL_Point &rhs) {
    return !(lhs == rhs);
}


void drawTile(SDL_Renderer *renderer, SDL_Texture *tilesetTexture, int tileIndex, SDL_Rect &destRect, int imgSizeX,
              int tileSize = 16) {
    int tilesPerRow = imgSizeX / tileSize;

    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = 32;

    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
}

void
renderTileset(SDL_Renderer *renderer, SDL_Texture *tileset, TTF_Font *font, SDL_Point imgSize, int tileSize = 16) {
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

            SDL_RenderCopy(renderer, tileset, &srcRect, &destRect);

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

std::vector<std::vector<int>> csvToMap(const string &filename) {
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


void WarState::Init() {
    uPath.clear();
    RS::getInstance().init(renderer);

    Map.push_back(csvToMap(BasePath "asset/map/pvp/bg.csv"));
    Map.push_back(csvToMap(BasePath"asset/map/pvp/map.csv"));

    if (!Map.empty() && !Map[0].empty()) {
        size_t numRows = Map[0].size();
        size_t numCols = Map[0][0].size();

        unitMap.resize(numRows);
        for (auto &row: unitMap) {
            row.resize(numCols, nullptr);
        }
    }
    unitMap.reserve(Map.capacity());
    Unit::setUnitMap(&unitMap);

    pathFinder = new PathFinder(Map, MapStats::getInstance(&Map));
    paths = new Paths(Map, MapStats::getInstance(&Map));

    texture = IMG_LoadTexture(renderer, BasePath"asset/graphic/NewTiles.png");
    if (!texture) {
        std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
    }
    RS::getInstance().setTexture(texture);
    Unit::setTexture(texture);
    texture = RS::getInstance().getTexture();

    infantryUnit = UnitFactory::createUnit(UnitType::INFANTRY, 2, 9, 2);
    mechUnit = UnitFactory::createUnit(UnitType::MECH, 1, 8, 2);
    infantryUnit2 = UnitFactory::createUnit(UnitType::INFANTRY, 13, 2, 1);

    unitMap[2][13] = infantryUnit2.get();
    unitMap[9][2] = infantryUnit.get();
    unitMap[8][1] = mechUnit.get();
    indexFont = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 10);
}

void WarState::UnInit() {
    delete (paths);
    delete (pathFinder);
}

bool WarState::HandleEvent(const Event &event) {
    if (event.type == SDL_MOUSEMOTION) {
        mousePos.x = event.motion.x / 32;
        mousePos.y = event.motion.y / 32;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        mouseDown = true;
    }
    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        mouseDown = false;
        sameClick = false;
    }


    auto unit = unitMap[mousePos.y][mousePos.x];
    if ((mouseDown && unit) || selected) {
        if (unit != nullptr && unit->getTeam() == 1) {
            selected = unit;
        }
        if (unit != nullptr && !selected && unit->getTeam() != 1) {

            uPath.clear();
            radius = paths->getMoveRadius(mousePos, unit->getMovementType(), unit->getMoveRange(),
                                          radius);

        } else if (selected) {
            radius = paths->getMoveRadius(selected->getCoordinates(),
                                          selected->getMovementType(),
                                          selected->getMoveRange(),
                                          radius);
            uPath = paths->getPath(selected->getCoordinates(), mousePos, radius);

            if (!sameClick && event.type == SDL_MOUSEBUTTONDOWN && event.button.button && paths->mouseInRadius(mousePos, radius) && mousePos != selected->getCoordinates()) {
                unitMap[selected->getCoordinates().y][selected->getCoordinates().x] = nullptr;
                unitMap[mousePos.y][mousePos.x] = selected;
                selected->setCoordinates(mousePos.x, mousePos.y);
                uPath.clear();
                radius.clear();
                selected = nullptr;
                sameClick = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT &&
                       !paths->mouseInRadius(mousePos, radius)) {
                uPath.clear();
                radius.clear();
                selected = nullptr;
            }

        }
    } else {
        uPath.clear();
        radius.clear();
    }

    return false;
}


void WarState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
}

void WarState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_Rect destRect;
    for (int layer = 0; layer < 2; layer++) {
        for (size_t j = 0; j < Map[layer].size(); j++) {
            for (size_t k = 0; k < Map[layer][j].size(); k++) {
                destRect.x = int(k) * 32;
                destRect.y = int(j) * 32;
                drawTile(renderer, texture, Map[layer][j][k], destRect, 512);
            }
        }
    }

    //if (selected)
    paths->drawMoveRadius(frame, radius);


    if (!uPath.empty()) {
        for (auto &point: uPath) {
            destRect.x = point.x * 32 + 8;
            destRect.y = point.y * 32 + 8;
            destRect.w = destRect.h = 16;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &destRect);

        }
    }
    infantryUnit2->draw();
    infantryUnit->draw();
    mechUnit->draw();
    //renderTileset(renderer, texture, indexFont, {512, 208});
}

