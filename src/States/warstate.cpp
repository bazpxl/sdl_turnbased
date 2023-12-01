#include <fstream>
#include <sstream>
#include <iostream>
#include "Units/Unit.h"
#include "Helper/map_stat_helper.h"
#include "examplegame.h"

SDL_Texture *texture;
std::vector<std::vector<std::vector<int>>> Map;
#define BasePath "../../"
TTF_Font* indexFont;


std::unique_ptr<Unit> infantryUnit;

void drawTile(SDL_Renderer *renderer, SDL_Texture *tilesetTexture, int tileIndex, SDL_Rect &destRect, int tileSize = 16,
              int tilesPerRow = 18) {
    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = 32;

    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
}

void renderTileset(SDL_Renderer *renderer, SDL_Texture *tileset,TTF_Font *font, int tileSize = 16, int tilesPerRow = 18, int tilesPerColumn = 11) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    srcRect.w = srcRect.h = tileSize;
    destRect.w = destRect.h = tileSize * 2;

    for (int y = 0; y < tilesPerColumn; ++y) {
        for (int x = 0; x < tilesPerRow; ++x) {
            // Setze die Quell-Rechteck-Koordinaten
            srcRect.x = x * tileSize;
            srcRect.y = y * tileSize;

            // Setze die Ziel-Rechteck-Koordinaten
            destRect.x = x * tileSize * 2;
            destRect.y = y * tileSize * 2;

            // Zeichne das Tile
            SDL_RenderCopy(renderer, tileset, &srcRect, &destRect);

            // Text zum Rendern des Indexes
            int index = y * tilesPerRow + x;
            std::string indexText = std::to_string(index);
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, indexText.c_str(), {255, 255, 255}); // Weißer Text
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            // Zeichne den Text auf das Tile
            SDL_Rect textRect = {destRect.x, destRect.y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            // Räume auf
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
    RS::getInstance().init(renderer);

    texture = IMG_LoadTexture(renderer, BasePath"asset/graphic/tileset.png");
    if (!texture) {
        std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
    }

    Map.push_back(csvToMap("../../asset/map/unittest/map_Background.csv"));
    Map.push_back(csvToMap("../../asset/map/unittest/map_Objects.csv"));

    infantryUnit = UnitFactory::createUnit(UnitType::INFANTRY, 0, 7, 3);
    indexFont = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 12);

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
                drawTile(renderer, texture, Map[layer][j][k], destRect);
            }
        }
    }
    //renderTileset(renderer, texture, indexFont);
    infantryUnit->draw(texture);
    SDL_Point p = infantryUnit->getCoordinates();
    int cost = MapStats::getInstance(&Map).getMovementCost(p.x, p.y,infantryUnit->getMovementType());
    int defense = MapStats::getInstance(&Map).getDefense(p.x, p.y);
    std::cout << "Cost: " << cost << " Defense: " << defense << std::endl;
}

