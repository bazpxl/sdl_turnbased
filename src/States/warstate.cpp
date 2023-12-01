#include <fstream>
#include <sstream>
#include <iostream>
#include "Units/Unit.h"
#include "examplegame.h"

SDL_Texture * texture;
std::vector<std::vector<std::vector<int>>> Map;
#define BasePath "../../"

std::unique_ptr<Unit> infantryUnit;

void drawTile(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileIndex, int tileSize, int tilesPerRow, SDL_Rect& destRect) {
    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = 32;

    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
}


std::vector<std::vector<int>> csvToMap(const string& filename) {
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
    RS::getInstance().init(renderer);

    texture = IMG_LoadTexture(renderer, BasePath"asset/graphic/tileset.png");
    if (!texture) {
        std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
    }

    Map.push_back(csvToMap("../../asset/map/unittest/map_Background.csv"));
    Map.push_back(csvToMap("../../asset/map/unittest/map_Objects.csv"));

    infantryUnit = UnitFactory::createUnit(UnitType::INFANTRY,3,4,3);
}

void WarState::UnInit() {

}

bool WarState::HandleEvent(const Event &event) {
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
                drawTile(renderer, texture, Map[layer][j][k], 16, 18, destRect);
            }
        }
    }
    infantryUnit->draw(texture);
}

