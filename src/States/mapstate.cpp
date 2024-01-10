#include "examplegame.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Units/Unit.h"
#include "Helper/map_stat_helper.h"
#include "examplegame.h"
#include "Map/Paths.h"


void MapState::drawTile(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileIndex, SDL_Rect& destRect, int imgSizeX,
    int tileSize = 16, int w = 32) {
    int tilesPerRow = imgSizeX / tileSize;

    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = w;

    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
}

void
MapState::renderTileset(SDL_Renderer* renderer, SDL_Texture* tileset, TTF_Font* font, SDL_Point imgSize, int tileSize = 16) {
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
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, indexText.c_str(), { 255, 255, 255, 255 });
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRect = { destRect.x, destRect.y, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void MapState::mapToCsv(const std::vector<std::vector<int>> map, const string& filename) {
    std::ofstream file;
    file.open(filename);

    for (size_t x = 0; x < map.size(); x++) {
        for (size_t y = 0; y < map[0].size(); y++) {
            if (map[0].size() - 1 == y)
                file << map[x][y] << "\n";
            else
                file << map[x][y] << ",";
        }
    }
    file.close();
}


std::vector<std::vector<int>> MapState::csvToMap(const string& filename) {
    std::vector<std::vector<int>> map;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Fehler beim offnen der Datei: " << filename << std::endl;
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


void MapState::Init() {
    RS::getInstance().init(renderer);

    Map.push_back(csvToMap(BasePath "asset/map/pvp/bg.csv"));
    Map.push_back(csvToMap(BasePath"asset/map/pvp/map.csv"));


    texture = IMG_LoadTexture(renderer, BasePath"asset/graphic/NewTiles.png");
    if (!texture) {
        std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
    }
    RS::getInstance().setTexture(texture);
    Unit::setTexture(texture);
    texture = RS::getInstance().getTexture();


}

void MapState::UnInit() {

}

bool MapState::HandleEvent(const Event& event) {

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int indexX = floor((((float)event.button.x) / 16));
        int indexY = floor((((float)event.button.y) / 16));
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (event.button.x < 288 && event.button.y < 208) {
                selected = (indexY * 32) + indexX;
                std::cout << "Selected " << selected << " at (" << indexX << "|" << indexY << ")\n";
            }
            indexX = floor((((float)event.button.x - 288) / 32));
            indexY = floor((((float)event.button.y) / 32));
            if (indexX >= 0 && indexX < Map[0][0].size() && indexY >= 0 && indexY < Map[0].size())
                Map[currentLayer][indexY][indexX] = selected;


            if (event.button.x > game.GetWindowSize().x - 32 && event.button.y < 16) {
                currentLayer = 1 - (game.GetWindowSize().x - event.button.x) / 16;
            }
            else if (event.button.x > game.GetWindowSize().x - 48 && event.button.y < 16) {
                string filename;
                std::cout << "Bitte geben Sie den Dateinamen zum speichern an\n";
                std::cin >> filename;
                mapToCsv(Map[0], BasePath "asset/map/" + filename + "_bg.csv");
                mapToCsv(Map[1], BasePath "asset/map/" + filename + "_map.csv");
            }
            else if (event.button.x > game.GetWindowSize().x - 64 && event.button.y < 16) {
                string filename;
                std::cout << "Bitte geben Sie den Dateinamen zum laden an\n";
                std::cin >> filename;
                Map[0] = csvToMap(BasePath "asset/map/" + filename + "_bg.csv");
                Map[1] = csvToMap(BasePath "asset/map/" + filename + "_map.csv");

            }

        }
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            indexX = floor((((float)event.button.x - 288) / 32));
            indexY = floor((((float)event.button.y) / 32));
            if (indexX >= 0 && indexX < Map[0][0].size() && indexY >= 0 && indexY < Map[0].size())
                Map[currentLayer][indexY][indexX] = -1;
        }
    }

    return false;
}


void MapState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
}

void MapState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_Rect toolbox = { 0 ,0,512,208 };
    SDL_RenderCopy(renderer, texture, NULL, &toolbox);


    SDL_Rect destRect;
    for (int layer = 0; layer < 2; layer++) {
        for (size_t j = 0; j < Map[layer].size(); j++) {
            for (size_t k = 0; k < Map[layer][j].size(); k++) {
                destRect.x = 288 + int(k) * 32;
                destRect.y = int(j) * 32;
                drawTile(renderer, texture, Map[layer][j][k], destRect, 512);
            }
        }
    }


    // Selection Box

    int borderX = (selected % 32) * 16;
    int borderY = floor(selected / 32) * 16;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect barz = { borderX,borderY,2,16 };
    SDL_RenderFillRect(renderer, &barz);
    barz = { borderX + 15,borderY,2,16 };
    SDL_RenderFillRect(renderer, &barz);
    barz = { borderX,borderY,16,2 };
    SDL_RenderFillRect(renderer, &barz);
    barz = { borderX,borderY + 15,16,2 };
    SDL_RenderFillRect(renderer, &barz);

    destRect = { game.GetWindowSize().x - 48,-16 };
    drawTile(renderer, texture, 321, destRect, 512);

    destRect = { game.GetWindowSize().x - 32,-16 };
    drawTile(renderer, texture, 322, destRect, 512);

    destRect = { game.GetWindowSize().x - 48,0,16,16 };
    drawTile(renderer, texture, 113, destRect, 512, 16, 16);

    destRect = { game.GetWindowSize().x - 64,0,16,16 };
    drawTile(renderer, texture, 81, destRect, 512, 16, 16);

    borderX = ((game.GetWindowSize().x - 32) + 16 * currentLayer);
    borderY = 0;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    barz = { borderX,borderY,2,16 };
    SDL_RenderFillRect(renderer, &barz);
    barz = { borderX + 15,borderY,2,16 };
    SDL_RenderFillRect(renderer, &barz);
    barz = { borderX,borderY,16,2 };
    SDL_RenderFillRect(renderer, &barz);
    barz = { borderX,borderY + 15,16,2 };
    SDL_RenderFillRect(renderer, &barz);

}

