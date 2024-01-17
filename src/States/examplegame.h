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
class ShopState;
class Player;

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
    Paths* paths;
    inline static Player *currentPlayer = nullptr;
    ActionMenu actionMenu;

    std::vector<Player*>players;
    std::vector<std::vector<std::vector<int>>> map;
    std::vector<std::vector<std::unique_ptr<Building>>> buildingMap;
	std::vector<SDL_Texture*>_panelFontTextures;
	std::vector<SDL_Texture*>_panelTextures;
    SDL_Texture *texture;

    // handle unit interaction
    SDL_Point mouseIndex;
    std::vector<std::vector<Unit *>> unitMap;
    std::vector<std::unique_ptr<Unit>> units;

    std::unordered_map<SDL_Point, int, Paths::SDLPointHash, Paths::SDLPointEqual> attackRadius;
    CombatCalculator * cc;

    bool initialisiert = false;

    Unit *selected = nullptr;
    bool mouseDown = false;
    bool sameClick = true;
    std::vector<Paths::Node> radius;
    std::vector<SDL_Point> path;

    inline static int shopUnit = 0;
    Point mousePositionShop;

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

	static bool isRightMouseButtonDown(const Event &event);
	static bool isRightMouseButtonUp(const Event &event);

	void handleLeftMouseButtonDown();
    void handleLeftMouseButtonUp();

	void handleRightMouseButtonDown();

    void clearSelectionAndPath();

    void handleUnitInteraction(Unit* unit, const Event &event);

    void processUnitSelectionAndMovement(const Event &event);

    void getFactory(const Event& event);
    void setBoughtUnit(const Event& event);
    void nextPlayer();
	void endRound();
};

class ShopState : public ExampleGameState
{
public:
    SDL_Surface* imageSurface;
    SDL_Texture* croppedTexture;
    TTF_Font* font;
    SDL_Texture* textTextureGuthaben;
    SDL_Point guthabenSize;
    std::array<SDL_Texture*, 2> textTextures;
    std::array<SDL_Point,2> textureSizes;

    Point p = { 15 * 32, 11 * 32 };
    Array<SDL_Rect, 2> unitRecs;
    Array<int, 2> prices = { 1000, 3000 };
    Array<SDL_Rect, 2> einheiten = { 16 * 16, 16 * 5, 16, 16, 17 * 16, 16 * 5, 16, 16 };
    Array<Unit, 2> units = { Infantry(3,2,1), Mech(4,5,1) };
    Array<char[20], 2> unitnames = { "Infantry", "Mech" };
    SDL_Color textColor = { 255, 255, 255, 255 };

    char textBuffer[250];
    using ExampleGameState::ExampleGameState;

    void Init() override;
    void UnInit() override;

    bool HandleEvent(const Event& event) override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};

class MapState : public ExampleGameState
{
public:
    // ctor
    using ExampleGameState::ExampleGameState;

    std::vector<std::vector<std::vector<int>>> Map;
    SDL_Texture* texture;

    void drawTile(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileIndex, SDL_Rect& destRect, int imgSizeX, int tileSize, int w);

    static void renderTileset(SDL_Renderer* renderer, SDL_Texture* tileset, TTF_Font* font, SDL_Point imgSize, int tileSize);

    static void mapToCsv(const std::vector<std::vector<int>> map, const string& filename);

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