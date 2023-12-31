#pragma once

#include <global.h>
#include <gamebase.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Helper/recthelper.h"
#include "Helper/autocast_ptr.h"
#include "Helper/render_singleton.h"
#include "player.h"
#include "Units/Unit.h"
#include "Helper/map_stat_helper.h"
#include "Map/Paths.h"
#include "Units/CombatCalculator.h"
#include "Buildings/Building.h"
#include "UI/ActionMenu.h"


class ExampleGame;

class MapState;
class WarState;

class ExampleGame final : public Game {
public:
    ExampleGame();

    bool HandleEvent(const Event &event) override;
};

class ExampleGameState : public GameState {
public:
    [[nodiscard]] ExampleGame &GetGame() const override { return dynamic_cast<ExampleGame &>( GameState::GetGame()); }

    using GameState::GameState;
};

class WarState : public ExampleGameState {
public:

	TTF_Font *_indexFont;
    ActionMenu actionMenu;

	Paths *paths;
    Player *currentPlayer;
    std::vector<Player*>players;
    std::vector<std::vector<std::vector<int>>> map;
    std::vector<std::vector<Building*>> buildingMap;
	std::vector<SDL_Texture*>_panelFontTextures;
	std::vector<SDL_Texture*>_panelTextures;
    SDL_Texture *texture;

    // handle unit interaction
    SDL_Point mouseIndex;
    std::vector<std::vector<Unit *>> unitMap;
    std::vector<std::unique_ptr<Unit>> units;

    std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> attackRadius;
    CombatCalculator * cc;

    Unit *selected = nullptr;
    bool mouseDown = false;
    bool sameClick = true;
    std::vector<Paths::Node> radius;
    std::vector<SDL_Point> path;

    using ExampleGameState::ExampleGameState;

    void Init() override;

    void UnInit() override;

    bool HandleEvent(const Event &event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    Color GetClearColor() const override { return Color{0, 0, 0, SDL_ALPHA_OPAQUE}; }

    // Initialization

    void loadTileset(const string &filename);

    void initMap();

    void initUnitMap();

    void initBuildingMap();

    void loadUnitMap();

    void saveUnitMap();

    static std::vector<std::vector<int>> csvToMap(const string &filename);

    // Drawing

    void drawTile(int tileIndex, SDL_Rect &destRect,int imgSizeX,int tileSizeInTileset = 16);

    void renderTileset(TTF_Font *font, SDL_Point imgSize, int tileSize = 16);

    void drawMap();

    void drawUnits();

	void drawInterface();

    // Event Handling
    void updateMouseIndex(const Event &event);

    static bool isLeftMouseButtonDown(const Event &event);

    static bool isLeftMouseButtonUp(const Event &event);

    void handleLeftMouseButtonDown();

    void handleLeftMouseButtonUp();

    void clearSelectionAndPath();

    void handleUnitInteraction(Unit* unit, const Event &event);

    void processUnitSelectionAndMovement(const Event &event);

    void nextPlayer();
	void endRound();
};

class MapState : public ExampleGameState
{
public:
    // ctor
    using ExampleGameState::ExampleGameState;

    std::vector<std::vector<std::vector<int>>> Map;
    SDL_Texture* texture;

    void drawTile(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileIndex, SDL_Rect& destRect, int imgSizeX, int tileSize, int w);

    void renderTileset(SDL_Renderer* renderer, SDL_Texture* tileset, TTF_Font* font, SDL_Point imgSize, int tileSize);

    void mapToCsv(const std::vector<std::vector<int>> map, const string& filename);

    std::vector<std::vector<int>> csvToMap(const string& filename);
    int selected = 0;
    int currentLayer = 0;
    void Init() override;
    void UnInit() override;

    bool HandleEvent(const Event& event) override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    Color GetClearColor() const override { return Color{ 0, 0, 0, SDL_ALPHA_OPAQUE }; }
};