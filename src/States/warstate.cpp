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


TTF_Font *indexFont;


std::unique_ptr<Unit> infantryUnit;

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
    unitMap[9][2] = infantryUnit.get();
    indexFont = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 10);
}

void WarState::UnInit() {
    delete (paths);
    delete (pathFinder);
}

bool WarState::HandleEvent(const Event &event) {
    // Mouse Update
    if (event.type == SDL_MOUSEMOTION) {
        mousePos.x = event.motion.x;
        mousePos.y = event.motion.y;

        // Wenn die Maus bewegt wird und eine Einheit ausgewählt ist
        if (selected) {
            if (paths->mouseInRadius(mousePos)) {
                uPath = paths->getPath(selected->getCoordinates(), {mousePos.x / 32, mousePos.y / 32},
                                       selected->getMovementType(),
                                       selected->getMoveRange());
            } else {
                uPath.clear();
            }
            radiusVisible = false;
        }
    }

    // Radius Stuff
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseIndexX = mousePos.x / 32;
        int mouseIndexY = mousePos.y / 32;

        if (unitMap[mouseIndexY][mouseIndexX]) {
            selected = unitMap[mouseIndexY][mouseIndexX];
            if (selected->getTeam() != 1/*currentPlayer->getTeam()*/) {
                radius = paths->getMoveRadius(selected->getCoordinates(), selected->getMovementType(),
                                              selected->getMoveRange());
                radiusVisible = true; // Zeige den Radius nur, wenn eine Einheit ausgewählt wird
            }
        } else {
            selected = nullptr;
            radius.clear();
            radiusVisible = false;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && selected) {
        selected->setCoordinates(mousePos.x / 32, mousePos.y / 32);
        radiusVisible = false;  // Verstecke den Radius nach dem Versetzen der Einheit
    }

    std::cout << MapStats::getInstance(&Map).getDefense(1,9) << std::endl;
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

    if(selected)
        paths->drawMoveRadius(frame);


    if (!uPath.empty()) {
        for (auto &point: uPath) {
            destRect.x = point.x * 32 + 8;
            destRect.y = point.y * 32 + 8;
            destRect.w = destRect.h = 16;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &destRect);

        }
    }
    infantryUnit->draw();
    //renderTileset(renderer, texture, indexFont, {512, 208});
}

